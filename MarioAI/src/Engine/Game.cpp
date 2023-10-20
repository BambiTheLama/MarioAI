#include "Game.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Blocks/Block.h"
#include "../GameObject/Player.h"
#include "ObjectToClone.h"
Game::Game()
{
	InitWindow(1600, 900, "MARIO");
	createObjectToCopy();
	Chunk* chunk = new Chunk(0,this);
	target = new Player({128,500,64,64},this);
	chunks.push_back(chunk);
	SetTargetFPS(60);
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
int i = 1;
void Game::update(float deltaTime)
{
	if (target)
	{
		target->update(deltaTime);
		camera.target.x = target->getPos().x;
	}
	for (auto chunk : chunks)
		chunk->update(deltaTime);

	if (IsKeyPressed(KEY_F1))
		loadChunk(i++);
	for (auto o : toDelete)
		delete o;
	toDelete.clear();

}


void Game::start()
{
	float deltaTime = 0;
	double time = 0;
	double time2 = GetTime();
	camera.offset = { GetScreenWidth() / 2.0f,GetScreenHeight() / 2.0f };
	camera.target.y = 880/2;
	camera.zoom = 1;
	while (!WindowShouldClose() && play)
	{
		time = time2;
		time2 = GetTime();
		deltaTime = time2 - time;

		update(deltaTime);
		BeginDrawing();
		ClearBackground(WHITE);
		BeginMode2D(camera);
		draw();
		EndMode2D();
		DrawFPS(0, 0);
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
	chunks.push_back(new Chunk(i,this));
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
bool Game::addBlock(GameObject* o)
{
	for (auto c : chunks)
		if (CheckCollisionRecs(o->getPos(), c->getPos()))
		{
			bool isAdd=c->addBlock(o);
			if (isAdd)
				return true;
		}
	return false;

}
void Game::addToDelete(GameObject* o)
{
	toDelete.remove(o);
	toDelete.push_back(o);
}
void Game::addObj(GameObject* o)
{
	for (auto c : chunks)
		if (CheckCollisionRecs(o->getPos(), c->getPos()))
			c->addObj(o);
}
void Game::updatePos(GameObject* o)
{
	bool isAtchunks = false;
	for (auto c : chunks)
	{
		if (CheckCollisionRecs(o->getPos(), c->getPos()))
		{
			if (!c->hasObj(o))
			{
				c->addObj(o);
			}
			isAtchunks = true;
		}
		else
		{
			if (c->hasObj(o))
			{
				c->removeObj(o);
			}
		}
	}
	if (!isAtchunks)
		addToDelete(o);
}