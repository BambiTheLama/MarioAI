#pragma once
#include "GameObject.h"
#include "ObjType/Hitable.h"
class Block :
    public GameObject
{
public:
	Block(Rectangle pos, std::string path,Game* game);

	Block(Block& b);

	virtual ~Block() {}

	virtual void update(float deltaTime);

	virtual ObjectType getType() { return ObjectType::Block; }

	virtual Block* clone() { return new Block(*this); }

};

