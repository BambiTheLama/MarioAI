#pragma once
#include "../GameObject.h"

class Goomba :
    public GameObject
{
    float sprite = 0;
    bool left = true;
public:
    Goomba(Rectangle pos, std::string path, Game* game);

    Goomba(Goomba& g);

    Goomba(nlohmann::json toRead);

    void update(float deltaTime);

    void draw();

    ObjectType getType() { return ObjectType::Enemy; }

    Goomba* clone() { return new Goomba(*this); }
};

