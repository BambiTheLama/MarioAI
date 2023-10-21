#include "Coin.h"
#include "../../Engine/Game.h"
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
			game->addPoints(200);
		}
		moveTo(pos.x, pos.y);
	}
	sprite += deltatime * 2;
}
void Coin::draw()
{
	Texture2D texture = getTexture().texture;
	Rectangle pos = getPos();
	Rectangle spritePos = { texture.height * ((int)sprite % 4),0,texture.height,texture.height };
	DrawTexturePro(texture, spritePos, pos, { 0,0 }, 0, WHITE);
}