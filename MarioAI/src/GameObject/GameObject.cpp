#include "GameObject.h"
#include "../Engine/Game.h"

std::list<TextureSource> GameObject::texturesLoaded=std::list<TextureSource>();

GameObject::GameObject(Rectangle pos, std::string path,Game* game)
{
	this->pos = pos;
	bool loaded = false;
	if (texturesLoaded.size() > 0)
	{
		for (auto t : texturesLoaded)
			if (path.compare(t.path) == 0)
			{
				this->texture = t;
				loaded = true;
			}
	}

	if (!loaded)
	{
		texture.texture = LoadTexture(path.c_str());
		texture.path = path;
		if (texture.texture.id > 0)
			texturesLoaded.push_back(texture);
	}
	this->game = game;
}

GameObject::GameObject(GameObject& o)
{
	pos = o.pos;
	texture = o.texture;
	game = o.game;
}

void GameObject::draw()
{
	Rectangle source = { 0,0,(float)texture.texture.width,(float)texture.texture.height };
	DrawTexturePro(texture.texture, source, pos, { 0,0 }, 0, WHITE);
}

void GameObject::readFromFile(nlohmann::json& readFile)
{
	pos.x = readFile["Pos"][0];
	pos.y = readFile["Pos"][1];
	pos.width = readFile["Pos"][2];
	pos.height = readFile["Pos"][3];
	fromStaticObjList = readFile["StaticObjList"];
	ID = readFile["ID"];
}

std::list<GameObject*> GameObject::getObjectsAt(Rectangle pos, ObjectType type)
{
	if (!game)
		return std::list<GameObject*>();
	std::list<GameObject*> obj = game->getObjectsAt(pos);
	obj.remove(this);
	std::list<GameObject*> retList;

	if (type == ObjectType::Player)
	{
		GameObject* o = game->getPlayer();
		if (o &&CheckCollisionRecs(pos,o->getPos()))
			retList.push_back(o);
	}
	else
	{
		for (auto o : obj)
		{
			if (!CheckCollisionRecs(o->getPos(), pos))
				continue;
			if (o->getType() != type)
				continue;
			retList.push_back(o);
		}
	}
	return retList;
}

bool GameObject::isObjectAt(Rectangle pos, ObjectType type)
{
	if (!game)
		return false;
	std::list<GameObject*> obj = game->getObjectsAt(pos);
	obj.remove(this);
	for (auto o : obj)
	{
		if (CheckCollisionRecs(o->getPos(), pos))
			if (o->getType() == type)
			{
				return true;
			}
	}
	return false;
}

void GameObject::updatePos()
{
	game->updatePos(this);
}
void GameObject::deleteObject()
{
	game->removeObj(this);
	game->addToDelete(this);
}

void GameObject::saveToFile(nlohmann::json& saveFile)
{
	saveFile["Pos"][0] = pos.x;
	saveFile["Pos"][1] = pos.y;
	saveFile["Pos"][2] = pos.width;
	saveFile["Pos"][3] = pos.height;
	saveFile["StaticObjList"] = fromStaticObjList;
	saveFile["ID"] = ID;
}