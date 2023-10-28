#include "GameLearnScene.h"
#include "Engine.h"
#include "MainMenu.h"

GameLearnScene::GameLearnScene()
{
	for (int g = 0; g < GenerationSize; g++)
		games[g] = new Game();
	currentGame = NULL;
	i = 0;
	SetExitKey(0);
}

GameLearnScene::~GameLearnScene()
{
	for (auto g : games)
		if(g)
			delete g;
}

void GameLearnScene::update(float deltaTime)
{

	if (currentGame)
	{
		currentGame->update(deltaTime);
		if (!currentGame->isPlaing())
		{
			for (int g = 0; g < i; g++)
			{
				if (games[g]->getFitnes() < currentGame->getFitnes())
				{
					Game* tmp = currentGame;
					currentGame = games[g];
					games[g] = tmp;
				}
			}
			games[i] = currentGame;
			i++;
			if (i >= GenerationSize)
			{
				newGeneration();
				i = 0;
			}
			currentGame = NULL;
		}
	}
	else
	{
		currentGame = games[i];
	}
	if (IsKeyPressed(KEY_ESCAPE))
		Engine::getEngine()->setScene(new MainMenu());
}

void GameLearnScene::newGeneration()
{
	NN* nns[GenerationSize];
	Game* gamesTmp[GenerationSize];
	int index = 0;
	int n = 10;
	for (int i = 0; i < n && i < GenerationSize; i++)
	{
		gamesTmp[index] = games[i];
		gamesTmp[index + 1] = games[i];
		gamesTmp[index + 2] = games[i];
		index += 3;
	}
	for (int i = n; i < n * 2 && i < GenerationSize; i++)
	{
		gamesTmp[index] = games[i];
		gamesTmp[index + 1] = games[i];
		index += 2;
	}
	for (int i = n*2; i < GenerationSize; i++)
	{
		gamesTmp[index] = games[i];
		index++;
	}
	for (int i = 0; i < GenerationSize; i++)
	{
		int g1 = rand() % GenerationSize;
		int g2 = 0;
		do {
			g2 = rand() % GenerationSize;
		} while (gamesTmp[g1] == gamesTmp[g2]);
		NN* n1 = gamesTmp[g1]->getNN();
		NN* n2 = gamesTmp[g2]->getNN();
		nns[i] = n1->combineNNs(n2);
		nns[i]->mutate();
	}


	for (int g = 0; g < GenerationSize; g++)
		delete games[g];
	for (int g = 0; g < GenerationSize; g++)
		games[g] = new Game(nns[g]);
	i = 0;
	generationNumber++;
}

void GameLearnScene::draw()
{
	if (currentGame)
		currentGame->draw();
	DrawText(TextFormat("Generation : %d (Game : %d) ",generationNumber,i), 0, GetScreenHeight() - 50, 20, BLACK);
}