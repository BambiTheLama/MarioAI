#pragma once
#include "GameObject.h"
class Flame :
    public GameObject
{
    bool left;
    bool hit = false;
    int sprite;
    float timer = 0;
    float lifeTime = 1;
    int upPos = 0;
public:
    Flame(Rectangle pos, std::string path, Game* game,bool left);

    Flame(Flame& f);

    void update(float deltaTime);

    void draw();

    void move(float deltaTime);

    ObjectType getType() { return ObjectType::PlayerProjectal; }

    Flame* clone() { return new Flame(*this); }
};

