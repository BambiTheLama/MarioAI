#pragma once
#include "../GameObject.h"
class Flag :
    public GameObject
{
public:
    Flag(Rectangle pos, std::string path, Game* game);

    Flag(Flag& f);

    void update(float deltaTime);

    void draw();

    ObjectType getType() { return ObjectType::Finish; }

    Flag* clone() { return new Flag(*this); }
};

