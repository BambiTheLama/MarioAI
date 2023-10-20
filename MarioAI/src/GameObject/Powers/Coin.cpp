#include "Coin.h"

Coin::Coin(Rectangle pos, std::string path, Game* game, bool atMap):GameObject(pos,path,game)
{
	this->atMap = atMap;
}

Coin::Coin(Coin& c):GameObject(c)
{
	this->atMap = c.atMap;
}

void Coin::update(float deltatime)
{
	if (atMap)
	{

	}
	else
	{
		Rectangle pos = getPos();
		pos.y -= deltatime * 128;
		timer -= deltatime;
		if (timer <= 0)
		{
			deleteObject();
		}
	}
}