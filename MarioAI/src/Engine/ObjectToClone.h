#pragma once
#include "../GameObject/GameObject.h"


enum class ObjectID
{
	Flor = 0,
	Lava,
	Brick,
	Steal,
	Pipe,
	PowerBlockCoin,
	PowerBlockMushroom,
	PowerBlockFlower,
};

void createObjectToCopy();
void deleteObjectToCopy();
GameObject* cloneObject(ObjectID Id);