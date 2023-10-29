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

	Game(bool AI);

	Game(NN* n);

	~Game();

	void draw();

	void drawPlayerOnly();

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

	int getFitnes() { return target->getFitnes(); }

	void beginCamareMode(){ BeginMode2D(camera); }

	void endCameraMode() { EndMode2D(); }
};

