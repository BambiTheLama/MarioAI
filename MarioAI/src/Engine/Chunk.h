#pragma once
#include "../json.hpp"
#include "raylib.h"
#include "../GameObject/Blocks/Block.h"
#include "ObjectToClone.h"
#include <vector>
#include <list>
#define blockSize 64
#define mapH 15
#define mapW 25


class Game;
class Chunk
{
	int chunk;
	GameObject* blocks[mapH][mapW];
	std::list<GameObject*> objects;
	std::list<GameObject*> toRemove;
	std::list<GameObject*> toDelete;
	std::list<GameObject*> toAdd;
	Game* game;
	bool showGrid = false;
public:
	Chunk(int chunk,Game* game);

	Chunk(int chunk);

	Chunk(int chunk, Game* game, nlohmann::json map);

	~Chunk();

	void update(float deltaTime);

	void draw();

	void getObjs(Rectangle pos, std::list<GameObject*>* obj);

	void addObj(GameObject* o) { toAdd.remove(o); toAdd.push_back(o); }

	bool addObj(GameObject* o, Vector2 pos);

	void removeObj(GameObject* o) { toRemove.remove(o); toRemove.push_back(o); }

	bool addBlock(GameObject* o);

	bool addBlock(GameObject* o, Vector2 pos);

	void removeBlock(GameObject* o);

	void removeAt(Vector2 pos);

	void updateLists();

	Rectangle getPos() { return { chunk * mapW * 64.0f,0,64.0f * mapW,64 * mapH }; }

	int getChunk() { return chunk; }

	bool hasObj(GameObject* o);

	void getAllObjects(std::list<GameObject*> &objs);

	void saveToJson(nlohmann::json &saveFile);

	void clearAllObjects();

	friend class MapEditor;
};

