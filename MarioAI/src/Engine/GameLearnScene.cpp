#include "GameLearnScene.h"
#include "Engine.h"
#include "MainMenu.h"
#include <fstream>
GameLearnScene::GameLearnScene()
{
	for (int g = 0; g < GenerationSize; g++)
		games[g] = new Game();
	readFromFile();
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
	if (fastModeOn)
		fastMode(deltaTime);
	else
		allGamesMode(deltaTime);

	if (IsKeyPressed(KEY_ESCAPE))
		Engine::getEngine()->setScene(new MainMenu());
}
void GameLearnScene::fastMode(float deltaTime)
{
	deltaTime *= 10;
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
}

void GameLearnScene::allGamesMode(float deltaTime)
{
	/*

	if (IsKeyPressed(KEY_A))
	{
		i = (i + GenerationSize - 1) % GenerationSize;
		if (i < 0)
			i = GenerationSize - 1;
		else if (i >= GenerationSize)
			i = 0;
	}
	if (IsKeyPressed(KEY_D))
	{
		i = (i + 1) % GenerationSize;
		if (i < 0)
			i = GenerationSize - 1;
		else if (i >= GenerationSize)
			i = 0;
	}
	if (IsKeyPressed(KEY_W))
	{
		
	}
		*/
	setTheFarestGame();
	for (int i = 0; i < GenerationSize; i++)
		if(games[i]->isPlaing())
			games[i]->update(deltaTime);
	if (isAllGamesEnd())
	{
		for (int i = 0; i < generationNumber; i++)
			for (int j = 0; j < generationNumber-1; j++)
			{
				if (games[j]->getFitnes() < games[j + 1]->getFitnes())
				{
					Game* g = games[j];
					games[j] = games[j + 1];
					games[j + 1] = g;
				}
			}
		newGeneration();
	}
}
void GameLearnScene::newGeneration()
{
	NN* nns[GenerationSize];
	Game* gamesTmp[GenerationSize];
	int index = 0;
	int n = GenerationSize / 16;
	printf("///////////////////////////////////////////////////\n");
	for (int i = 0; i < GenerationSize && index < GenerationSize; i++)
	{
		for (int k = 0; k < n && index < GenerationSize; k++)
		{
			gamesTmp[index] = games[i];
			index++;
		}
		printf("%lf\n", games[i]->getFitnes());
		n--;
		if (n < 1)
			n = 1;
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

	saveNNToFile();
}

void GameLearnScene::draw()
{
	if (fastModeOn)
	{
		if (currentGame)
			currentGame->draw();
	}
	else
	{
		games[i]->draw();
		games[i]->beginCamareMode();
		for (int i = 0; i < this->i; i++)
			games[i]->drawPlayerOnly();
		for (int i = this->i + 1; i < GenerationSize; i++)
			games[i]->drawPlayerOnly();
		games[i]->endCameraMode();
	}

	DrawText(TextFormat("Generation : %d (Game : %d) ",generationNumber,i), 0, GetScreenHeight() - 50, 20, BLACK);
}

void GameLearnScene::saveNNToFile()
{
	nlohmann::json j;
	j["Generation"] = generationNumber;
	for (int i = 0; i < GenerationSize; i++)
		games[i]->getNN()->saveToFile(j["NEURONS"][i]);
	std::ofstream writer;
	writer.open("NN.json");
	writer << j;
	writer.close();
}

void GameLearnScene::readFromFile()
{
	std::ifstream reader;
	reader.open("NN.json");
	if (reader.is_open())
	{
		nlohmann::json j;
		reader >> j;
		for (int g = 0; g < GenerationSize && g < j["NEURONS"].size(); g++)
		{
			if (games[g])
				delete games[g];
			games[g] = new Game(new NN(j["NEURONS"][g]));
		}
		generationNumber = j["Generation"];

	}
}

bool GameLearnScene::isAllGamesEnd()
{
	for (int i = 0; i < GenerationSize; i++)
		if (games[i]->isPlaing())
			return false;
	return true;
}

void GameLearnScene::setTheFarestGame()
{
	int f = games[0]->getFitnes();
	i = 0;
	for(int i=1;i<GenerationSize;i++)
		if (games[i]->getFitnes() > f)
		{
			this->i = i;
			f = games[i]->getFitnes();
		}

}

int GameLearnScene::activesGames()
{
	int n = 0;
	for (int i = 0; i < GenerationSize; i++)
		if (games[i]->isPlaing())
			n++;
	return n;
}