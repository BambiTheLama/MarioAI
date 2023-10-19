#include "PowerBlock.h"
#include "Game.h"
#include "PowerMushroom.h"
#include "PowerFlower.h"

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
	if (!used)
	{
		sprite += deltaTime;
		sprite -= ((int)sprite/2 )*2;
	}


}

void PowerBlock::draw()
{
	Texture2D texture = getTexture().texture;
	Rectangle texturePos = { (int)(sprite*2) * texture.height,0,texture.height,texture.height };
	Rectangle pos = getPos();
	DrawTexturePro(texture, texturePos, pos, { 0,0 }, 0, WHITE);
}

void PowerBlock::destory(int power)
{
	if (used)
		return;
	GameObject* o=NULL;
	Rectangle pos = getPos();
	pos.y -= pos.height;
	switch (powerType)
	{
	case PowerType::Coin:
		
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
	used = true;
	sprite = 1;
	game->removeBlock(this);
	game->addToDelete(this);
	o=cloneObject(3);
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