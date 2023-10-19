#pragma once
#include "GameObject.h"
class PowerMushroom :
    public GameObject
{
    float left = false;
public:
    PowerMushroom(Rectangle pos, std::string path, Game* game);

    PowerMushroom(PowerMushroom& f);

    void update(float deltaTime);

    ObjectType getType() { return ObjectType::PowerUp; }

    PowerMushroom* clone() { return new PowerMushroom(*this); }
};

