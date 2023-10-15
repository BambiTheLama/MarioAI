#pragma once
#include "GameObject.h"
class Game;
class Player :
    public GameObject
{
    float speed = 64;
    bool jumping = false;
    float jumpHeight = 500;
    float pressJumpTime = 0;
    float pressJumpTimeMax = 10.0f;
    Game* game = NULL;
public:
    Player(Rectangle pos,std::string path,Game *g);

    Player(Player& m);

    void draw();

    void update(float deltaTime);

    ObjectType getType() { return ObjectType::Palyer; }

    Player* clone() { return new Player(*this); }

};

