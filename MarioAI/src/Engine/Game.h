#pragma once
#include "raylib.h"
#include "../GameObject/GameObject.h"
#include <list>
#include "../json.hpp"
#include "Chunk.h"


class Game
{
	std::list<Chunk*> chunks;
	GameObject* target;
	nlohmann::json map;
	Camera2D camera;
	bool play = true;
	std::list<GameObject*> toDelete;
public:
	Game();

	~Game();

	void start();

	void draw();

	void update(float deltaTime);

	void deleteChunk(int i);

	void loadChunk(int i);

	std::list<GameObject*> getObjectsAt(Rectangle pos);

	void removeObj(GameObject* o);

	void removeBlock(GameObject* o);

	bool addBlock(GameObject* o);

	void addObj(GameObject* o);

	void addToDelete(GameObject* o);

	void lostGame() { play = false; }

	void updatePos(GameObject* o);
};

