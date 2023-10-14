#pragma once
#include "GameObject.h"
class Block :
    public GameObject
{
public:
	Block(Rectangle pos, std::string path);

	virtual ~Block() {}

	virtual void update();

	virtual ObjectType getType() { return ObjectType::Block; }

	virtual void draw();
};

