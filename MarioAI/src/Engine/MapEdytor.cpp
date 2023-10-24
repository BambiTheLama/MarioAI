#include "MapEdytor.h"
#include <fstream>
#include <iostream>
#include "MainMenu.h"
#include "Engine.h"

MapEdytor::MapEdytor()
{
	camera.offset = { GetScreenWidth() / 2.0f,GetScreenHeight() / 2.0f };
	camera.target.y = 880 / 2;
	camera.target.x = GetScreenWidth() / 2.0f;
	camera.zoom = 1;
	menuPos = { (float)(GetScreenWidth() - 200),0,200,800 };
	for (int i = 0; i < (int)StaticObjectID::size; i++)
	{
		GameObject* o = cloneStaticObject((StaticObjectID)i);
		if (!o)
			continue;
		o->setPos({ menuPos.x + 20 + 84 * (i % 2),menuPos.y + 20 + 84 * (i / 2),64,64 });
		blocks.push_back(o);
	}
	for (int i = 0; i < (int)DynamicObjectID::size; i++)
	{
		GameObject* o = cloneDynamicObject((DynamicObjectID)i);
		if (!o)
			continue;
		o->setPos({ menuPos.x + 20 + 84 * (i % 2),menuPos.y + 20 + 84 * (i / 2),64,64 });
		otherObjects.push_back(o);
	}

	SetExitKey(0);
	std::ifstream reader("Map1.json");
	if (reader.is_open())
	{
		nlohmann::json j;
		reader >> j;
		for (int i = 0; i < j.size(); i++)
		{
			chunks.push_back(new Chunk(i, NULL, j));
		}
	}
	else
	{
		chunks.push_back(new Chunk(0));
	}
	for (auto c : chunks)
		c->showGrid = true;
}

MapEdytor::~MapEdytor()
{
	saveMap();
	for (auto o : blocks)
		delete o;
	for (auto o : otherObjects)
		delete o;
	if (usingObj)
		delete usingObj;
	for (auto c : chunks)
		delete c;
}

void MapEdytor::update(float delataTime)
{
	if (IsKeyPressed(KEY_TAB))
		menuLeft = !menuLeft;
	if (IsKeyPressed(KEY_ONE))
		displayBlock = false;
	if (IsKeyPressed(KEY_TWO))
		displayBlock = true;
	if (IsKeyPressed(KEY_A))
		camera.target.x += 64;
	if (IsKeyPressed(KEY_D))
		camera.target.x -= 64;
	if (IsKeyPressed(KEY_ENTER))
		newChunk();
	if (IsKeyPressed(KEY_DELETE))
		deleteChunk();

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		pressMouse();
	}
	else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		placeBlock();
	}

	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
	{
		removeObject();
	}
	else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
	{
		removeObject();
	}
	if (IsKeyPressed(KEY_ESCAPE))
		Engine::getEngine()->setScene(new MainMenu());

}

void MapEdytor::pressMouse()
{
	bool pressedAtMenu = false;
	if (menuLeft)
	{
		if (CheckCollisionPointRec(GetMousePosition(), menuPos))
			pressedAtMenu = true;
	}
	if (pressedAtMenu)
	{
		std::vector<GameObject*> l;
		if (displayBlock)
		{
			l = blocks;
		}
		else
		{
			l = otherObjects;
		}
		for (int i = 0; i < l.size(); i++)
		{
			if (!CheckCollisionPointRec(GetMousePosition(), l[i]->getPos()))
			{
				continue;
			}
			if (usingObj)
				delete usingObj;
			if (displayBlock)
			{
				usingBlock = true;
				usingObj = cloneStaticObject((StaticObjectID)i);
			}
			else
			{
				usingBlock = false;
				usingObj = cloneDynamicObject((DynamicObjectID)i);
			}

		}
	} 
	placeBlock();
}

void MapEdytor::placeBlock()
{

	if (!usingObj || CheckCollisionPointRec(GetMousePosition(), menuPos))
		return;
	Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
	for (auto c : chunks)
	{
		if (!CheckCollisionPointRec(mousePos, c->getPos()))
		{
			continue;
		}
		GameObject* o = usingObj->clone();
		if (usingBlock)
		{
			if (!c->addBlock(o, mousePos))
			{
				delete o;
			}
		}
		else
		{
			c->addObj(o, mousePos);
		}

	}
}

void MapEdytor::removeObject()
{
	bool pressedAtMenu = false;
	if (menuLeft)
	{
		if (CheckCollisionPointRec(GetMousePosition(), menuPos))
			pressedAtMenu = true;
	}
	if (pressedAtMenu)
		return;
	Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
	for (auto c : chunks)
	{
		if (!CheckCollisionPointRec(mousePos, c->getPos()))
		{
			continue;
		}
		c->removeAt(mousePos);
		c->update(0);

	}

}
void MapEdytor::newChunk()
{
	chunks.push_back(new Chunk(chunks.size()));
}

void MapEdytor::deleteChunk()
{
	if (chunks.size() <= 0)
		return;
	Chunk* c = chunks.back();
	chunks.pop_back();
	delete c;
}

void MapEdytor::saveMap()
{

	nlohmann::json map;
	for (auto c : chunks)
		c->saveToJson(map);
	std::ofstream writer("Map1.json");
	if (!writer.is_open())
		return;
	std::cout << map.dump(1);
	writer << map;
	writer.close();	
}

void MapEdytor::draw()
{
	BeginMode2D(camera);
	for (auto c : chunks)
		c->draw();
	EndMode2D();
	if (menuLeft)
	{
		DrawRectangleRec(menuPos, RED);
		std::vector<GameObject*> l;
		if (displayBlock)
		{
			l = blocks;	
		}
		else
		{
			l = otherObjects;
		}
		for (auto o : l)
		{
			Rectangle pos = o->getPos();
			o->draw();
			DrawRectangleLinesEx(pos, 2, BLACK);
		}


	}
	if (usingObj)
	{
		usingObj->moveTo(GetMouseX(), GetMouseY());
		usingObj->draw();
	}
	
}