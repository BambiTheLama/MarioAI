#pragma once
#include "../GameObject.h"
#include "../ObjType/Hitable.h"
class Coopa :
    public GameObject, public Hitable
{
    int sprite = 0;
    float timer = 0;
    bool left = true;
    bool alive = true;
    bool isShellMoving = false;
    float lifeTime = 1;
    bool haveWings = false;
public:
    Coopa(Rectangle pos, std::string path, Game* game);

    Coopa(Coopa& g);

    void update(float deltaTime);

    void moveNormal(float deltaTime);

    void moveShell(float deltaTime);

    void playerHit();

    void hitAllObjects();

    void draw();

    bool hitObj();

    ObjectType getType() { return ObjectType::Enemy; }

    Coopa* clone() { return new Coopa(*this); }

};

