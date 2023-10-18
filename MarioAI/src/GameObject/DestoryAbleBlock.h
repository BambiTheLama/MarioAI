#pragma once
#include "Block.h"
#include "ObjType/Destoryable.h"
class DestoryAbleBlock :
    public Block,public Destoryable
{

public:
    DestoryAbleBlock(Rectangle pos, std::string path, Game* game);

    DestoryAbleBlock(DestoryAbleBlock& d);

    virtual DestoryAbleBlock* clone() { return new DestoryAbleBlock(*this); }

    virtual void draw()
    {
        DrawRectangleRec(getPos(), RED);
        GameObject::draw();
    }

    virtual void destory();
};

