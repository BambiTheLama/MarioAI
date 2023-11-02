#pragma once
#include "Block.h"
#include "../ObjType/Destroyable.h"
class DestroyableBlock :
    public Block,public Destroyable
{

public:
    DestroyableBlock(Rectangle pos, std::string path, Game* game);

    DestroyableBlock(DestroyableBlock& d);

    virtual DestroyableBlock* clone() { return new DestroyableBlock(*this); }

    virtual void draw()
    {
        DrawRectangleRec(getPos(), RED);
        GameObject::draw();
    }

    virtual void destroy(int power);
};

