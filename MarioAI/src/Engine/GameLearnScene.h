#pragma once
#include "Scene.h"
#include "Game.h"
#define GenerationSize 100

class GameLearnScene :
    public Scene
{
    Game* games[GenerationSize];
    Game* currentGame;
    int i = 0;
    int generationNumber = 0;
public:
    GameLearnScene();

    ~GameLearnScene();

    void update(float deltaTime);

    void newGeneration();

    void draw();

    void saveNNToFile();

    void readFromFile();

};

