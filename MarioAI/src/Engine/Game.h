#pragma once
#include "raylib.h"
#include "../GameObject/GameObject.h"
#include <list>
#include "../json.hpp"
#include "Chunk.h"
#include "Scene.h"
#include "../NN.h"
#include "../GameObject/Player.h"
class Game:
	public Scene
{
	Rectangle cameraArea;
	std::list<Chunk*> chunks;
	Player* target;
	nlohmann::json map;
	Camera2D camera;
	bool play = true;
	bool win = false;
	std::list<GameObject*> toDelete;
	int points = 0;

public:
	Game();

	Game(NN* n);

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

	void setWin() { win = true; play = false; }

	void addPoints(int p) { points += p; }

	Player* getPlayer() { return target; }

	NN* getNN() { return target->getNeuronNetwork(); }

	bool isPlaing() { return play; }

	float getFitnes() { return target->getFitnes(); }
};

