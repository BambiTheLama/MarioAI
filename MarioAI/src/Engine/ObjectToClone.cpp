#include "ObjectToClone.h"
#include <vector>
#include "Chunk.h"
#include "../GameObject/Blocks/Flag.h"
#include "../GameObject/Blocks/Lava.h"
#include "../GameObject/Blocks/PowerBlock.h"
#include "../GameObject/Blocks/DestoryAbleBlock.h"
#include "../GameObject/ObjType/PowerType.h"
#include "../GameObject/Powers/Coin.h"
#include "../GameObject/Enemy/Goomba.h"
#include "../GameObject/Enemy/Coopa.h"
std::vector<GameObject*> staticObjectToCopy;
std::vector<GameObject*> dynamicObjectToCopy;

void createObjectToCopy()
{
	staticObjectToCopy = std::vector<GameObject*>{
		new Block({ 0,0,blockSize,blockSize }, "res/Stone.png",NULL),
		new Lava({ 0,0,blockSize,blockSize }, "res/Lava.png",NULL),
		new DestoryAbleBlock({0,0,blockSize,blockSize},"res/Brick.png",NULL),
		new Block({ 0,0,blockSize,blockSize }, "res/SteelBlock.png",NULL),
		new PowerBlock({ 0,0,blockSize,blockSize }, "res/CoinBlock.png",NULL,PowerType::Coin),
		new PowerBlock({ 0,0,blockSize,blockSize }, "res/CoinBlock.png",NULL,PowerType::Mushroom),
		new PowerBlock({ 0,0,blockSize,blockSize }, "res/CoinBlock.png",NULL,PowerType::Plant),
		new Coin({0,0,blockSize,blockSize},"res/Coin.png",NULL),

	};
	dynamicObjectToCopy = std::vector<GameObject*>{
		new Block({ 0,0,blockSize * 2,blockSize * 2 }, "res/Pipe.png",NULL),
		new Flag({0,0,blockSize,blockSize * 6},"res/Flag.png",NULL),
		new Goomba({0,0,blockSize,blockSize},"res/Goomba.png",NULL),
		new Coopa({0,0,blockSize,blockSize},"res/RedCoopa.png",NULL),
		new Coopa({0,0,blockSize,blockSize},"res/GrennCoopa.png",NULL),
	};
}
void deleteObjectToCopy()
{
	for (auto o : staticObjectToCopy)
	{
		delete o;
	}
	staticObjectToCopy.clear();
	
}
GameObject* cloneStaticObject(StaticObjectID Id)
{
	if ((int)Id < 0 || (int)Id >= staticObjectToCopy.size())
		return NULL;
	return staticObjectToCopy[(int)Id]->clone();
}

GameObject* cloneDynamicObject(DynamicObjectID Id)
{
	if ((int)Id < 0 || (int)Id >= dynamicObjectToCopy.size())
		return NULL;
	return dynamicObjectToCopy[(int)Id]->clone();
}