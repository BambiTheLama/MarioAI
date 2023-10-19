#pragma once
#include "GameObject.h"
class PowerFlower :
    public GameObject
{
public:
    PowerFlower(Rectangle pos, std::string path, Game* game);

    PowerFlower(PowerFlower& f);

    void update(float deltaTime);

    ObjectType getType() { return ObjectType::PowerUp; }

    PowerFlower* clone() { return new PowerFlower(*this); }
};

