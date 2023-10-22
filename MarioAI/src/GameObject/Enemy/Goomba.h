#pragma once
#include "../GameObject.h"
#include "../ObjType/Hitable.h"
class Goomba :
    public GameObject,public Hitable
{
    float sprite = 0;
    bool left = true;
    bool alive = true;
    float lifeTime = 1;
public:
    Goomba(Rectangle pos, std::string path, Game* game);

    Goomba(Goomba& g);

    Goomba(nlohmann::json toRead);

    void update(float deltaTime);

    void draw();

    ObjectType getType() { return ObjectType::Enemy; }

    Goomba* clone() { return new Goomba(*this); }

    bool hitObj();
};

