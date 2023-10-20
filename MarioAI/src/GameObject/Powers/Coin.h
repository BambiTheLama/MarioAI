#pragma once
#include "../GameObject.h"
#include "../ObjType/PowerType.h"
class Coin :
    public GameObject, public PowerTypeObject
{
    bool atMap;
    float timer = 2;
public:
    Coin(Rectangle pos, std::string path, Game* game,bool atMap=true);

    Coin(Coin& c);

    void update(float deltatime);

    ObjectType getType() { return ObjectType::PowerUp; }

    Coin* clone() { return new Coin(*this); }

    PowerType getPower() { return PowerType::Coin; }
};

