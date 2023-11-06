#pragma once
#include "Scene.h"
#include "Game.h"
#include <thread>
#include <fstream>

#define GenerationSize 200
struct LearningData
{
    int generationID;
    float maxFitness;
    int howManyWin;
    void save(std::ofstream &writer)
    {
        writer << generationID << " ; " << maxFitness << " ; " << howManyWin << "\n";
    }
    void read(std::ifstream& reader)
    {
        std::string s;
        reader >> generationID >> s >> maxFitness >> s >> howManyWin;
    }
};
class GameLearnScene :
    public Scene
{
    Game* games[GenerationSize];
    Game* currentGame;
    int i = 0;
    int generationNumber = 0;
    bool fastModeOn = true;
    float speed = 1;
    bool drawScene = true;
    int mapID;
    std::vector<LearningData> learnData;
public:
    GameLearnScene(int mapID);

    ~GameLearnScene();

    void update(float deltaTime);

    void setSpeedMode(int i);

    void fastMode(float deltaTime);

    void allGamesMode(float deltaTime);

    void newGeneration();

    void draw();

    void saveNNToFile();

    void readFromFile();

    bool isAllGamesEnd();

    void setTheFarestGame();

    int activesGames();

};

