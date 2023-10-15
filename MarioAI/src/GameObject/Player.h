#pragma once
#include "GameObject.h"
#include "ObjType/Hitable.h"

class Game;

class Player :
    public GameObject,public Hitable
{
    float speed = 32;
    bool jumping = false;
    float jumpHeight = 400;
    float pressJumpTime = 0;
    float pressJumpTimeMax = 10.0f;
    int hp = 1;
    float invisibleFrames = 0;
public:
    Player(Rectangle pos,std::string path,Game *g);

    Player(Player& m);

    void draw();

    void update(float deltaTime);

    ObjectType getType() { return ObjectType::Palyer; }

    Player* clone() { return new Player(*this); }

    void hitObj();

};

