#pragma once
#include "../json.hpp"
#include "raylib.h"
#include "Block.h"
#include <vector>
#include <list>
#define blockSize 64
#define mapH 15
#define mapW 25

void createObjectToCopy();
void deleteObjectToCopy();
class Game;
class Chunk
{
	int chunk;
	GameObject* blocks[mapH][mapW];
	std::list<GameObject*> objects;
	std::list<GameObject*> toRemove;
	Game* game;

public:
	Chunk(int chunk,Game* game);

	Chunk(int chunk, Game* game, nlohmann::json map);

	~Chunk();

	void update(float deltaTime);

	void draw();

	void getObjs(Rectangle pos, std::list<GameObject*>* obj);

	void addObj(GameObject* o) { objects.remove(o); objects.push_back(o); }

	void removeObj(GameObject* o) { toRemove.remove(o); toRemove.push_back(o); }

	void removeBlock(GameObject* o);

	Rectangle getPos() { return { chunk * mapW * 64.0f,0,64.0f * mapW,64 * mapH }; }

	int getChunk() { return chunk; }
};

