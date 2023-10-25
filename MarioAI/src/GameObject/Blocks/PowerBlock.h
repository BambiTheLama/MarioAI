#pragma once
#include "Block.h"
#include "../ObjType/Destoryable.h"
#include "../ObjType/PowerType.h"
class PowerBlock :
    public Block,public Destoryable
{
    float sprite = 0;
    PowerType powerType;
public:
    PowerBlock(Rectangle pos, std::string path, Game* game, PowerType type);

    PowerBlock(PowerBlock& d);

    virtual void update(float deltaTime);

    virtual PowerBlock* clone() { return new PowerBlock(*this); }

    virtual void draw();

    virtual void destory(int power);
};

