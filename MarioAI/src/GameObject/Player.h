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
    int sprite = 0;
    int points = 0;
    float distance = 0;
    TextureSource bigPlayer;
    TextureSource powerPlayer;
    bool moveLeft = false;
    bool jumpFromEnemy = false;
    float flameCdr=0;
public:
    Player(Rectangle pos, Game* g);

    Player(Player& m);

    void draw();

    void update(float deltaTime);

    void checkPowerUps();

    ObjectType getType() { return ObjectType::Player; }

    Player* clone() { return new Player(*this); }

    bool hitObj();

};

