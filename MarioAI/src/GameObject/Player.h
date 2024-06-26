#pragma once
#include "GameObject.h"
#include "ObjType/Hitable.h"
#include "../NN.h"
#include "../Engine/Chunk.h"
class Game;


class Player :
    public GameObject,public Hitable
{
    float speed = 32;
    bool jumping = false;
    float jumpHeight = blockSize*6;
    float pressJumpTime = 0;
    float pressJumpTimeMax = 6.0f;
    int hp = 1;
    float invisibleFrames = 0;
    int sprite = 0;
    int points = 0;
    float distance = 0;
    TextureSource bigPlayer;
    TextureSource powerPlayer;
    bool moveLeft = false;
    bool jumpFromEnemy = false;
    float flameCdr=0;
    bool AI = true;
    float deltaTime = 0;
    float fitness = 0;
    float endProcesTimer = 2;
    float endProcesTimerMax = 2;
    NN *nn=NULL;
public:
    Player(Rectangle pos, Game* g, bool AI = true);

    Player(Rectangle pos, Game* g, NN* n);

    Player(Player& m);

    ~Player();

    void draw();

    void drawInterface();

    void update(float deltaTime);

    void aiControll();

    void playerControll();

    void objectInteration();

    void pressA();

    void pressD();

    void pressSpace();

    void pressS();

    void pressControl();

    void checkPowerUps();

    ObjectType getType() { return ObjectType::Player; }

    Player* clone() { return new Player(*this); }

    bool hitObj();

    void setNeuronNetwork(NN* nn);

    NN* getNeuronNetwork() { return nn; }

    float getFitness() { return fitness; }
};

