#include "MapEdytor.h"
#include <fstream>
#include <iostream>
MapEdytor::MapEdytor()
{
	camera.offset = { GetScreenWidth() / 2.0f,GetScreenHeight() / 2.0f };
	camera.target.y = 880 / 2;
	camera.target.x = GetScreenWidth() / 2.0f;
	camera.zoom = 1;
	menuPos = { (float)(GetScreenWidth() - 200),0,200,800 };
	printf("\n\n%d\n\n", sizeof(StaticObjectID));
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
	chunks.push_back(new Chunk(0));
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
	if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		pressMouse();
	}


}

void MapEdytor::pressMouse()
{
	bool pressedAtMenu = false;
	if (menuLeft)
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
			pressedAtMenu = true;
			if (usingObj)
				delete usingObj;
			if (displayBlock)
				usingObj = cloneStaticObject((StaticObjectID)i);
			else
				usingObj = cloneDynamicObject((DynamicObjectID)i);
		}
	}
	if (pressedAtMenu || !usingObj)
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

void MapEdytor::newChunk()
{
	chunks.push_back(new Chunk(chunks.size()));
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