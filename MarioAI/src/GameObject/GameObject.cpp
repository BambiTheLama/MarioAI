#include "GameObject.h"
#include "Game.h"

std::list<textureSource> GameObject::texturesLoaded=std::list<textureSource>();

GameObject::GameObject(Rectangle pos, std::string path,Game* game)
{
	this->pos = pos;
	bool loaded = false;
	if (texturesLoaded.size() > 0)
	{
		for (auto t : texturesLoaded)
			if (path.compare(t.path) == 0)
			{
				this->texture = texture;
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

std::list<GameObject*> GameObject::getObjectsAt(Rectangle pos, ObjectType type)
{
	if (!game)
		return std::list<GameObject*>();
	std::list<GameObject*> obj = game->getObjectsAt(pos);
	obj.remove(this);
	std::list<GameObject*> retList;
	for (auto o : obj)
	{
		if (!CheckCollisionRecs(o->getPos(), pos))
			continue;
		if (o->getType() != type)
			continue;
		retList.push_back(o);
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