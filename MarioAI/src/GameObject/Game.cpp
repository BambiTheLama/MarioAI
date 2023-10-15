#include "Game.h"
#include "GameObject.h"
#include "Block.h"
Game::Game()
{
	InitWindow(1600, 896, "MARIO");
	createObjectToCopy();
	Chunk* chunk = new Chunk(0);
	chunks.push_back(chunk);

}

Game::~Game()
{
	for (auto chunk : chunks)
		delete chunk;
	for (auto t : GameObject::texturesLoaded)
		UnloadTexture(t.texture);
	GameObject::texturesLoaded.clear();
	CloseWindow();
	deleteObjectToCopy();
}

void Game::draw()
{
	for(auto chunk:chunks)
		chunk->draw();
	if (target)
		target->draw();
}

void Game::update(float deltaTime)
{
	for (auto chunk : chunks)
		chunk->update(deltaTime);
	if(target)
		target->update(deltaTime);
}


void Game::start()
{
	float deltaTime = 0;
	double time = 0;
	double time2 = GetTime();
	while (!WindowShouldClose())
	{
		time = time2;
		time2 = GetTime();
		deltaTime = time2 - time;

		update(deltaTime);
		BeginDrawing();
		ClearBackground(WHITE);
		draw();
		EndDrawing();
	}
}

void Game::deleteChunk(int i)
{
	Chunk* chunk = NULL;
	for(auto c:chunks)
		if (c->getChunk() == i)
		{
			chunk = c;
			break;
		}
	if (!chunk)
		return;
	delete chunk;

}

void Game::loadChunk(int i)
{
	chunks.push_back(new Chunk(i, map));
}

std::list<GameObject*> Game::getObjectsAt(Rectangle pos)
{
	std::list<GameObject*> objs;
	for (auto c : chunks)
		c->getObjs(pos, &objs);
	return objs;
}

void Game::removeObj(GameObject* o)
{
	for (auto c : chunks)
		c->removeObj(o);
}

void Game::removeBlock(GameObject* o)
{
	for (auto c : chunks)
		if (CheckCollisionRecs(o->getPos(), c->getPos()))
			c->removeBlock(o);
}
void Game::addObj(GameObject* o)
{
	for (auto c : chunks)
		if (CheckCollisionRecs(o->getPos(), c->getPos()))
			c->addObj(o);
}
