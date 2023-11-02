#include "PowerBlock.h"
#include "../../Engine/Game.h"
#include "../Powers/PowerMushroom.h"
#include "../Powers/PowerFlower.h"
#include "../Powers/Coin.h"

PowerBlock::PowerBlock(Rectangle pos, std::string path, Game* game,PowerType powerType):Block(pos,path,game)
{
	this->powerType = powerType;
}

PowerBlock::PowerBlock(PowerBlock& d):Block(d)
{
	this->powerType = d.powerType;
}

void PowerBlock::update(float deltaTime)
{
	sprite += deltaTime;
	sprite -= ((int)sprite / 2) * 2;
}

void PowerBlock::draw()
{
	Texture2D texture = getTexture().texture;
	Rectangle texturePos = { (int)(sprite*2) * texture.height,0,texture.height,texture.height };
	Rectangle pos = getPos();
	DrawTexturePro(texture, texturePos, pos, { 0,0 }, 0, WHITE);
}

void PowerBlock::destroy(int power)
{
	GameObject* o=NULL;
	Rectangle pos = getPos();
	pos.y -= pos.height;
	switch (powerType)
	{
	case PowerType::Coin:
		o = new Coin(pos, "res/Coin.png", game,false);
		break;
	case PowerType::Mushroom:
		o = new PowerMushroom(pos, "res/Mushroom.png", game);
		break;
	case PowerType::Plant:
		o = new PowerFlower(pos, "res/Flower.png", game);
		break;
	default:
		break;
	}
	if (o)
		game->addObj(o);
	sprite = 1;
	game->removeBlock(this);
	o=cloneStaticObject(StaticObjectID::Steal);
	if (o)
	{
		Rectangle pos = getPos();
		o->moveTo(pos.x, pos.y);
		if (!game->addBlock(o))
		{
			delete o;
		}
	}

}