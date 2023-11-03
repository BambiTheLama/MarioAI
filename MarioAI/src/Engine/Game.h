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
	bool AI = true;
	float timer = 0.0f;
	float maxTimer = 180.0f;
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

	bool isWin() { return win; }

	int getFitness() { return target->getFitness() + (win ? (3000 * (maxTimer - timer) / maxTimer) : 0); }

	void beginCamareMode(){ BeginMode2D(camera); }

	void endCameraMode() { EndMode2D(); }
};

