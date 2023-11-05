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
	timer = maxTimer;
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
	reader.close();
	target = new Player({ 128,blockSize * (mapH - 4),64,64 }, this);

	camera.offset = { GetScreenWidth() / 2.0f,GetScreenHeight() / 2.0f };
	camera.target.y = 880 / 2;
	camera.zoom = 1;

}
Game::Game(bool AI)
{
	timer = maxTimer;
	std::ifstream reader("Map1.json");
	if (reader.is_open())
	{
		nlohmann::json j;
		reader >> j;
		for (int i = 0; i < j.size(); i++)
		{
			chunks.push_back(new Chunk(i, this, j));
		}
	}
	else
	{
		Chunk* chunk = new Chunk(0, this);
		chunks.push_back(chunk);
	}
	reader.close();
	target = new Player({ 128,blockSize * (mapH - 4),64,64 }, this,AI);

	camera.offset = { GetScreenWidth() / 2.0f,GetScreenHeight() / 2.0f };
	camera.target.y = 880 / 2;
	camera.zoom = 1;
	this->AI = AI;
}
Game::Game(NN* n):Game()
{
	if (target)
		delete target;
	target = new Player({ 128,blockSize * (mapH - 4),64,64 }, this,n);
}
Game::~Game()
{
	std::list<GameObject*> objs;
	for (auto chunk : chunks)
	{
		chunk->getAllObjects(objs);
		chunk->clearAllObjects();
		delete chunk;
	}

	for (auto d : toDelete)
	{
		objs.remove(d);
		objs.push_back(d);
	}
	for (auto o : objs)
		delete o;
	delete target;
	target == NULL;

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
	DrawTextEx(CustomFont::customFont, TextFormat("Timer: %d:%d", (int)timer / 60, (int)timer % 60), Vector2{ GetScreenWidth() - 200.0f,0 }, 64, 0, BLACK);
	if (target)
		target->drawInterface();


}

void Game::drawPlayerOnly()
{
	if (target)
		target->draw();
}

void Game::update(float deltaTime)
{
	timer -= deltaTime;
	if (timer <= 0)
	{
		lostGame();
		return;
	}
	if (IsKeyPressed(KEY_F))
	  ToggleFullscreen();
	if (!AI)
	{
		if (IsKeyPressed(KEY_ESCAPE))
		{
			Engine::getEngine()->setScene(new MainMenu());
			return;
		}

	}
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
		for (auto chunk : chunks)
			chunk->updateLists();
		for (auto o : toDelete)
			delete o;
		toDelete.clear();
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