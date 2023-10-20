#include "ObjectToClone.h"
#include "../GameObject/Blocks/Lava.h"
#include "../GameObject/Blocks/DestoryAbleBlock.h"
#include "../GameObject/Blocks/PowerBlock.h"
#include <vector>
#include "Chunk.h"
#include "../GameObject/ObjType/PowerType.h"
#include "../GameObject/Powers/Coin.h"

std::vector<GameObject*> objectToCopy;

void createObjectToCopy()
{
	objectToCopy = std::vector<GameObject*>{
		new Block({ 0,0,blockSize,blockSize }, "res/Stone.png",NULL),
		new Lava({ 0,0,blockSize,blockSize }, "res/Lava.png",NULL),
		new DestoryAbleBlock({0,0,blockSize,blockSize},"res/Brick.png",NULL),
		new Block({ 0,0,blockSize,blockSize }, "res/SteelBlock.png",NULL),
		new Block({ 0,0,blockSize,blockSize }, "res/Pipe.png",NULL),
		new PowerBlock({ 0,0,blockSize,blockSize }, "res/CoinBlock.png",NULL,PowerType::Coin),
		new PowerBlock({ 0,0,blockSize,blockSize }, "res/CoinBlock.png",NULL,PowerType::Mushroom),
		new PowerBlock({ 0,0,blockSize,blockSize }, "res/CoinBlock.png",NULL,PowerType::Plant),
		new Coin({0,0,blockSize,blockSize},"res/Coin.png",NULL),


	};
}
void deleteObjectToCopy()
{
	for (auto o : objectToCopy)
	{
		delete o;
	}
	objectToCopy.clear();
	
}
GameObject* cloneObject(ObjectID Id)
{
	if ((int)Id < 0 || (int)Id >= objectToCopy.size())
		return NULL;
	return objectToCopy[(int)Id]->clone();
}