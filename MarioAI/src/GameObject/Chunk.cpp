#include "Chunk.h"
#include "Lava.h"
#include "DestoryAbleBlock.h"
std::vector<GameObject*> objectToCopy;

void createObjectToCopy()
{
	objectToCopy = std::vector<GameObject*>{
	new Block({ 0,0,blockSize,blockSize }, "res/Block.png",NULL),
	new Lava({ 0,0,blockSize,blockSize }, "res/Lava.png",NULL),
	new DestoryAbleBlock({0,0,blockSize,blockSize},"res/Block.png",NULL),
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

Chunk::Chunk(int chunk, Game* game)
{
	this->chunk = chunk;
	for (int y = 0; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
			blocks[y][x] = NULL;
	float startX = chunk * mapW * blockSize;
	for (int y = mapH-3; y < mapH-1; y++)
		for (int x = 0; x < mapW; x++)
		{
			blocks[y][x] = objectToCopy[0]->clone();
			blocks[y][x]->moveTo(startX + x * blockSize, y * blockSize);
			blocks[y][x]->setGame(game);
		}
	for (int y = mapH - 1; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
		{
			blocks[y][x] = objectToCopy[1]->clone();
			blocks[y][x]->moveTo(startX + x * blockSize, y * blockSize);
			blocks[y][x]->setGame(game);
		}
	this->game = game;
	blocks[7][10] = objectToCopy[2]->clone();
	blocks[7][10]->moveTo(startX + 10 * blockSize, 7 * blockSize);
	blocks[7][10]->setGame(game);
}

Chunk::Chunk(int chunk, Game* game, nlohmann::json map)
{
	this->chunk = chunk;
	for (int y = 0; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
			blocks[y][x] = NULL;
	if (map.size() > chunk)
	{
		return;
	}
	float startX = chunk * mapW * blockSize;
	int i = 0;
	if (map[chunk]["Blocks"].size() >= i)
		return;
	int blockId = map[chunk]["Blocks"][i][0];
	int times = map[chunk]["Blocks"][i][1];
	for (int y = 0; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
		{
			blocks[y][x] = objectToCopy[blockId]->clone();
			blocks[y][x]->moveTo(startX + x * blockSize, y * blockSize);
			blocks[y][x]->setGame(game);
			times--;
			if (times <= 0)
			{
				i++;
				if (map[chunk]["Blocks"].size() >= i)
					return;
				blockId = map[chunk]["Blocks"][i][0];
				times = map[chunk]["Blocks"][i][1];
			}
		}
	for (int y = mapH - 1; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
		{
			blocks[y][x] = objectToCopy[1]->clone();
			blocks[y][x]->moveTo(startX + x * blockSize, y * blockSize);
			blocks[y][x]->setGame(game);
		}
	this->game = game;
}

Chunk::~Chunk()
{
	for (int y = 0; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
			if (blocks[y][x])
				delete blocks[y][x];
	for (auto o : objects)
		delete o;
}

void Chunk::update(float deltaTime)
{
	for (int y = 0; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
			if (blocks[y][x])
				blocks[y][x]->update(deltaTime);
	for (auto o : objects)
		o->update(deltaTime);
}

void Chunk::draw()
{
	for (int y = 0; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
			if (blocks[y][x])
				blocks[y][x]->draw();
	for (auto o : objects)
		o->draw();
}

void Chunk::getObjs(Rectangle pos, std::list<GameObject*>* obj)
{
	if (!CheckCollisionRecs(getPos(), pos))
		return;
	for(auto o:objects)
		if (CheckCollisionRecs(pos, o->getPos()))
		{
			obj->remove(o);
			obj->push_back(o);
		}
	int x = (pos.x - (chunk * mapW * 64)) / 64;
	int y = pos.y / 64;
	int w = x + pos.width / 64;
	int h = y + pos.height / 64;
	int startX = mapW-1;
	int startY = mapH-1;
	if (x >= 0 && x < mapW)
		startX = x;
	if (y >= 0 && y < mapH)
		startY = y;
	int endX = mapW-1;
	int endY = mapH-1;
	if (w >= 0 && w < mapW)
		endX = w;
	if (h >= 0 && h < mapH)
		endY = h;
	for(int x=startX;x<=endX;x++)
		for(int y=startY;y<=endY;y++)
			if(blocks[y][x])
				obj->push_back(blocks[y][x]);
}

void Chunk::removeBlock(GameObject* o)
{
	Rectangle pos = o->getPos();
	int x = (pos.x - (chunk * mapW * 64)) / 64;
	int y = pos.y / 64;
	if (x < 0 || x >= mapW || y < 0 || y >= mapH)
		return;
	//delete blocks[y][x];
	blocks[y][x] = NULL;
}