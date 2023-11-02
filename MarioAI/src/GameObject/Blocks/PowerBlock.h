#pragma once
#include "Block.h"
#include "../ObjType/Destroyable.h"
#include "../ObjType/PowerType.h"
class PowerBlock :
    public Block,public Destroyable
{
    float sprite = 0;
    PowerType powerType;
public:
    PowerBlock(Rectangle pos, std::string path, Game* game, PowerType type);

    PowerBlock(PowerBlock& d);

    virtual void update(float deltaTime);

    virtual PowerBlock* clone() { return new PowerBlock(*this); }

    virtual void draw();

    virtual void destroy(int power);
};

