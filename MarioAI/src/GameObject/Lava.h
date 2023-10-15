#pragma once
#include "GameObject.h"
class Lava :
    public GameObject
{
public:
	Lava(Rectangle pos, std::string path,Game* game);

	Lava(Lava& l);

	virtual ~Lava() {}

	virtual void update(float deltaTime);

	virtual ObjectType getType() { return ObjectType::Lava; }

	virtual Lava* clone() { return new Lava(*this); }
};

