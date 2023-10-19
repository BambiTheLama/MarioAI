#pragma once
#include "GameObject.h"
#include "ObjType/PowerType.h"
class PowerMushroom :
    public GameObject,public PowerTypeObject
{
    float left = false;
public:
    PowerMushroom(Rectangle pos, std::string path, Game* game);

    PowerMushroom(PowerMushroom& f);

    void update(float deltaTime);

    ObjectType getType() { return ObjectType::PowerUp; }

    PowerMushroom* clone() { return new PowerMushroom(*this); }

    PowerType getPower() { return PowerType::Mushroom; }
};

