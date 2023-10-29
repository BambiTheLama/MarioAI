#pragma once
#include "Scene.h"
#include "Game.h"
#include <thread>

#define GenerationSize 200

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
public:
    GameLearnScene();

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

