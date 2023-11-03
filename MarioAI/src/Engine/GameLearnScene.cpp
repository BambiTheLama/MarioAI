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
	setSpeedMode(1);
}

GameLearnScene::~GameLearnScene()
{
	saveNNToFile();
	for (int g = 0; g < GenerationSize; g++)
		if(games[g])
			delete games[g];
}

void GameLearnScene::update(float deltaTime)
{
	deltaTime = 1.0f / 30.0f;
	if (IsKeyPressed(KEY_F1))
		drawScene = !drawScene;
	if (fastModeOn)
		fastMode(deltaTime);
	else
		allGamesMode(deltaTime);

	if (IsKeyPressed(KEY_ESCAPE))
		Engine::getEngine()->setScene(new MainMenu());
}
void GameLearnScene::setSpeedMode(int mode)
{
	if (mode == 0)
	{
		SetTargetFPS(1);
	}
	else if (mode == 1)
	{
		SetTargetFPS(15);
	}
	else if (mode == 2)
	{
		SetTargetFPS(30);
	}
	else if (mode==3)
	{
		SetTargetFPS(60);
	}
	else if (mode == 4)
	{
		SetTargetFPS(90);
	}
	else if (mode == 5)
	{
		SetTargetFPS(120);
	}
	else if (mode == 6)
	{
		SetTargetFPS(240);
	}
	else if (mode == 7)
	{
		SetTargetFPS(580);
	}
	else if (mode == 8)
	{
		SetTargetFPS(1160);
	}
	else if (mode == 9)
	{
		SetTargetFPS(99999999);
	}
}
void GameLearnScene::fastMode(float deltaTime)
{
	int k = GetKeyPressed();
	if (k >= '0' && k <= '9')
		setSpeedMode(k - '0');
	if (currentGame)
	{
		currentGame->update(deltaTime);
		if (!currentGame->isPlaing())
		{
			for (int g = 0; g < i; g++)
			{
				if (games[g]->getFitness() < currentGame->getFitness())
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

	setTheFarestGame();
	for (int i = 0; i < GenerationSize; i++)
		if(games[i]->isPlaing())
			games[i]->update(deltaTime);
	if (isAllGamesEnd())
	{
		for (int i = 0; i < generationNumber; i++)
			for (int j = 0; j < generationNumber-1; j++)
			{
				if (games[j]->getFitness() < games[j + 1]->getFitness())
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
	//int n = GenerationSize / sqrt(GenerationSize);
	int n = GenerationSize * 5.0f/ 100.0f;
	LearningData l;
	l.generationID = generationNumber;
	l.howManyWin = 0;
	l.maxFitness = games[0]->getFitness();
	for (int i = 0; i < GenerationSize; i++)
	{
		if (games[i]->isWin())
		{
			l.howManyWin++;
		}
	}
	learnData.push_back(l);

	nlohmann::json j;
	j["Generation"] = generationNumber;
	printf("///////////////////////////////////////////////////\n");
	for (int i = 0; i < 25 && i < GenerationSize; i++)
	{
		j["NNS"][i]["Fit"] = (int)games[i]->getFitness();
		games[i]->getNN()->saveToFile(j["NNS"][i]);
	}
	std::ofstream writer;
	writer.open(std::string("NN/NN" + std::to_string(generationNumber) + ".json"));
	writer << j;
	writer.close();
	j.clear();
	for (int i = 0; i < GenerationSize && index < GenerationSize; i++)
	{
		for (int k = 0; k < n && index < GenerationSize; k++)
		{
			gamesTmp[index] = games[i];
			index++;
		}
		printf("%d\n", games[i]->getFitness());


		

		n--;
		if (n < 1)
			n = 1;
	}
	for (int i = 0; i < 5; i++)
	{
		nns[i] = new NN(*games[i]->getNN());
	}
	for (int i = 5; i < GenerationSize; i++)
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

	//saveNNToFile();
}

void GameLearnScene::draw()
{
	if (!drawScene)
		return;
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
	DrawRectangle(0, GetScreenHeight() - 60, 300, 40, WHITE);
	DrawTextEx(CustomFont::customFont, TextFormat("Generation : %d (Game : %d) ", generationNumber, i), Vector2{ 0, (float)GetScreenHeight() - 60 }, 32, 2, BLACK);

}

void GameLearnScene::saveNNToFile()
{
	nlohmann::json j;
	j["Generation"] = generationNumber;
	for (int i = 0; i < GenerationSize; i++)
		games[i]->getNN()->saveToFile(j["NNS"][i]);
	std::ofstream writer;
	writer.open("NN.json");
	writer << j;
	writer.close();

	writer.open("LearnningData.txt");
	writer << "GenerationID" << " ; " << "maxFitness" << " ; " << "howManyWin" << "\n";
	for (auto l : learnData)
		l.save(writer);
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
		for (int g = 0; g < GenerationSize && g < j["NNS"].size(); g++)
		{
			if (games[g])
				delete games[g];
			games[g] = new Game(new NN(j["NNS"][g]));
		}
		generationNumber = j["Generation"];

	}
	reader.close();
	reader.open("LearnningData.txt");
	learnData.clear();
	if (reader.is_open())
	{
		std::string s;

		reader >> s >> s >> s >> s >> s;
		while (!reader.eof())
		{
			LearningData l;
			l.read(reader);
			learnData.push_back(l);
		}

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
	int f = games[0]->getFitness();
	i = 0;
	for(int i=1;i<GenerationSize;i++)
		if (games[i]->getFitness() > f)
		{
			this->i = i;
			f = games[i]->getFitness();
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