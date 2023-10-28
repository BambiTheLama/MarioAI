#include "Game.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Blocks/Block.h"
#include "../GameObject/Player.h"
#include "ObjectToClone.h"
#include "Engine.h"
#include "MainMenu.h"
#include <fstream>
Game::Game()
{
	std::ifstream reader("Map1.json");
	if (reader.is_open())
	{
		nlohmann::json j;
		reader >> j;
		for (int i = 0; i < j.size(); i++)
		{
			chunks.push_back(new Chunk(i, this,j));
		}
	}
	else
	{
		Chunk* chunk = new Chunk(0, this);
		chunks.push_back(chunk);
	}

	target = new Player({ 128,500,64,64 }, this);

	camera.offset = { GetScreenWidth() / 2.0f,GetScreenHeight() / 2.0f };
	camera.target.y = 880 / 2;
	camera.zoom = 1;

}

Game::Game(NN* n):Game()
{
	if (target)
		delete target;
	target = new Player({ 128,500,64,64 }, this,n);
}
Game::~Game()
{
	for (auto chunk : chunks)
		delete chunk;
	for (auto d : toDelete)
		delete d;
	delete target;

}

void Game::draw()
{
	BeginMode2D(camera);
	for (auto chunk : chunks)
		if (CheckCollisionRecs(cameraArea, chunk->getPos()))
			chunk->draw();
		
	if (target)
		target->draw();
	EndMode2D();
	if (target)
		target->drawInterface();

}
int i = 1;
void Game::update(float deltaTime)
{

	if (play)
	{
		if (target)
		{
			target->update(deltaTime);
			camera.target.x = target->getPos().x;
			cameraArea = { camera.target.x - GetScreenWidth() / 2,0,(float)GetScreenWidth(),(float)GetScreenHeight() };
		}

		for (auto chunk : chunks)
			if (CheckCollisionRecs(cameraArea, chunk->getPos()))
				chunk->update(deltaTime);
		for (auto o : toDelete)
			delete o;
		toDelete.clear();
	}



}


void Game::start()
{
	
	while (IsKeyUp(KEY_ENTER) && win && !WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(DARKBLUE);

		draw();

		DrawFPS(0, 0);

		DrawText("YOU PASS THE GAME\nPRESS ENDER TO CONTINUE", 100, 100, 69, WHITE);
	
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