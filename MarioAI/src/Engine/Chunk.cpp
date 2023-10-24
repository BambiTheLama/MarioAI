#include "Chunk.h"



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
			blocks[y][x] = cloneStaticObject(StaticObjectID::Flor);
			blocks[y][x]->moveTo(startX + x * blockSize, y * blockSize);
			blocks[y][x]->setGame(game);
		}
	for (int y = mapH - 1; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
		{
			blocks[y][x] = cloneStaticObject(StaticObjectID::Lava);
			blocks[y][x]->moveTo(startX + x * blockSize, y * blockSize);
			blocks[y][x]->setGame(game);
		}
	this->game = game;
	blocks[7][10] = cloneStaticObject(StaticObjectID::PowerBlockFlower);
	blocks[7][10]->moveTo(startX + 10 * blockSize, 7 * blockSize);
	blocks[7][10]->setGame(game);

	blocks[6][11] = cloneStaticObject(StaticObjectID::PowerBlockMushroom);
	blocks[6][11]->moveTo(startX + 11 * blockSize, 6 * blockSize);
	blocks[6][11]->setGame(game);

	blocks[mapH-4][0] = cloneStaticObject(StaticObjectID::Flor);
	blocks[mapH-4][0]->moveTo(startX + 0 * blockSize, (mapH - 4) * blockSize);
	blocks[mapH-4][0]->setGame(game);
	blocks[mapH - 4][mapW-1] = cloneStaticObject(StaticObjectID::Flor);
	blocks[mapH - 4][mapW-1]->moveTo(startX + (mapW-1) * blockSize, (mapH - 4) * blockSize);
	blocks[mapH - 4][mapW-1]->setGame(game);

	GameObject* o = cloneDynamicObject(DynamicObjectID::Flag);
	o->moveTo(startX + (mapW - 1) * blockSize, (5) * blockSize);
	o->setGame(game);
	addObj(o);

	o = cloneDynamicObject(DynamicObjectID::Goomba);
	o->moveTo(startX + (mapW - 6) * blockSize, (mapH-5) * blockSize);
	o->setGame(game);
	addObj(o);
	o = cloneDynamicObject(DynamicObjectID::CoopaRed);
	o->moveTo(startX + (mapW - 4) * blockSize, (mapH - 5) * blockSize);
	o->setGame(game);
	addObj(o);

}
Chunk::Chunk(int chunk)
{
	this->chunk = chunk;
	float startX = chunk * mapW * blockSize;
	for (int y = 0; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
			blocks[y][x] = NULL;
	for (int y = mapH - 1; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
		{
			blocks[y][x] = cloneStaticObject(StaticObjectID::Lava);
			blocks[y][x]->moveTo(startX + x * blockSize, y * blockSize);
			blocks[y][x]->setGame(game);
		}
	objects = std::list<GameObject*>();
	toRemove = std::list<GameObject*>();
	toDelete = std::list<GameObject*>();
	toAdd = std::list<GameObject*>();
	showGrid = true;
}
Chunk::Chunk(int chunk, Game* game, nlohmann::json map)
{
	this->game = game;
	this->chunk = chunk;
	for (int y = 0; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
			blocks[y][x] = NULL;

	float startX = chunk * mapW * blockSize;
	int i = 0;
	if (map[chunk]["Blocks"].size() > i)
	{
		int blockId = map[chunk]["Blocks"][i][0];
		int times = map[chunk]["Blocks"][i][1];
		for (int y = 0; y < mapH; y++)
			for (int x = 0; x < mapW; x++)
			{
				blocks[y][x] = cloneStaticObject((StaticObjectID)blockId);
				if (blocks[y][x])
				{
					blocks[y][x]->moveTo(startX + x * blockSize, y * blockSize);
					blocks[y][x]->setGame(game);
				}

				times--;
				if (times <= 0)
				{
					i++;
					if (map[chunk]["Blocks"].size() <= i)
						break;
					blockId = map[chunk]["Blocks"][i][0];
					times = map[chunk]["Blocks"][i][1];
				}
			}
		for (int y = mapH - 1; y < mapH; y++)
			for (int x = 0; x < mapW; x++)
			{
				blocks[y][x] = cloneStaticObject(StaticObjectID::Lava);
				blocks[y][x]->moveTo(startX + x * blockSize, y * blockSize);
				blocks[y][x]->setGame(game);
			}
	}



	for (int i = 0; i < map[chunk]["Objects"].size(); i++)
	{

		int ID = map[chunk]["Objects"][i]["ID"];
		GameObject* o=NULL;

		o = cloneDynamicObject((DynamicObjectID)ID);

		if (o)
		{
			o->setGame(game);
			o->readFromFile(map[chunk]["Objects"][i]);
			objects.push_back(o);
		}

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
	for (auto o : toDelete)
		delete o;
	toAdd.clear();
	toRemove.clear();
	toDelete.clear();
}

void Chunk::update(float deltaTime)
{
	for (int y = 0; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
			if (blocks[y][x])
				blocks[y][x]->update(deltaTime);

	if (objects.size() > 0)
	{
		for (auto o : objects)
			o->update(deltaTime);
	}
	if (toRemove.size() > 0)
	{
		for (auto o : toRemove)
			objects.remove(o);
		toRemove.clear();
	}
	if (toDelete.size() > 0)
	{
		for (auto o : toDelete)
		{
			objects.remove(o);
			delete o;
		}
		toDelete.clear();
	}
	if (toAdd.size() > 0)
	{
		for (auto o : toAdd)
			objects.push_back(o);
		toAdd.clear();
	}





}

void Chunk::draw()
{
	for (int y = 0; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
			if (blocks[y][x])
				blocks[y][x]->draw();
	for (auto o : objects)
		o->draw();
	if (!showGrid)
		return;
	float startX = chunk * mapW * blockSize;

	for (int y = 0; y < mapH+1; y++)
		DrawLine(startX, y * blockSize, startX + mapW * blockSize, y * blockSize, BLACK);
	for (int x = 0; x < mapW+1; x++)
		DrawLine(startX + x * blockSize, 0, startX + x * blockSize, mapH * blockSize, BLACK);
	DrawText(TextFormat("%d", chunk), startX, 0, 20, RED);
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
	int x = ((pos.x - (chunk * mapW * 64)) / 64) - 1;
	int y = (pos.y / 64) - 1;
	int w = x + pos.width / 64 +2;
	int h = y + pos.height / 64 +2;
	int startX = mapW-1;
	int startY = mapH-1;
	if (x < 0)
		startX = 0;
	if (y < 0)
		startY = 0;
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

void Chunk::addObj(GameObject* o, Vector2 pos)
{
	objects.push_back(o);
	pos.x -= chunk * mapW * 64.0f;
	int x = (int)pos.x / 64;
	int y = (int)pos.y / 64;
	o->moveTo(chunk * mapW * 64.0f + x * 64, y * 64);
}

bool Chunk::addBlock(GameObject* o)
{
	Rectangle pos = o->getPos();
	int x = ((pos.x - (chunk * mapW * 64)) / 64);
	int y = (pos.y / 64);
	if (blocks[y][x])
	{
		return false;
	}
	blocks[y][x] = o;
	return true;
}

bool Chunk::addBlock(GameObject* o, Vector2 pos)
{
	pos.x -= chunk * mapW * 64.0f;
	int x = (int)pos.x/64;
	int y = (int)pos.y/64;
	if (x < 0 || x >= mapW || y < 0 || y >= mapH)
		return false;
	o->moveTo(chunk * mapW * 64.0f + x * 64, y * 64);
	if (blocks[y][x])
	{
		return false;
	}
	blocks[y][x] = o;
	return true;
}

void Chunk::removeBlock(GameObject* o)
{
	Rectangle pos = o->getPos();
	int x = (pos.x - (chunk * mapW * 64)) / 64;
	int y = pos.y / 64;
	if (x < 0 || x >= mapW || y < 0 || y >= mapH)
		return;
	if (o != blocks[y][x])
		return;
	toDelete.push_back(blocks[y][x]);
	blocks[y][x] = NULL;
}

void Chunk::removeAt(Vector2 pos)
{
	int x = (pos.x - (chunk * mapW * 64)) / 64;
	int y = pos.y / 64;
	if (x < 0 || x >= mapW || y < 0 || y >= mapH)
		return;
	if (blocks[y][x])
	{
		toDelete.push_back(blocks[y][x]);
		blocks[y][x] = NULL;
	}
	else
	{
		for (auto o : objects)
		{
			if (CheckCollisionPointRec(pos, o->getPos()))
			{
				toDelete.push_back(o);	
			}
		}
	}
}

bool Chunk::hasObj(GameObject* o)
{ 
	for (auto obj : objects)
		if (o == obj)
			return true; 
	return false; 
}

void Chunk::saveToJson(nlohmann::json& saveFile)
{
	int ID = -1;
	int times = 0;
	int i = 0;
	for (int y = 0; y < mapH; y++)
		for (int x = 0; x < mapW; x++)
		{
			int newId = ID;
			if (blocks[y][x])
			{
				newId = blocks[y][x]->getID();
			}
			else
			{
				newId = -1;
			}
			if (newId == ID)
			{
				times++;
			}
			else
			{
				saveFile[chunk]["Blocks"][i][0] = ID;
				saveFile[chunk]["Blocks"][i][1] = times;
				ID = newId;
				times = 1;
				i++;
			}
		}
	saveFile[chunk]["Blocks"][i][0] = ID;
	saveFile[chunk]["Blocks"][i][1] = times;
	i = 0;
	for (auto o : objects)
	{
		o->saveToFile(saveFile[chunk]["Objects"][i]);
		i++;

	}
	for (auto o : toAdd)
	{
		o->saveToFile(saveFile[chunk]["Objects"][i]);
		i++;

	}
}