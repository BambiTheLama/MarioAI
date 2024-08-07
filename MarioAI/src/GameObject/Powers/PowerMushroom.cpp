#include "PowerMushroom.h"
#include "../../Engine/Game.h"
PowerMushroom::PowerMushroom(Rectangle pos, std::string path, Game* game) :GameObject(pos, path, game)
{

}

PowerMushroom::PowerMushroom(PowerMushroom& f) :GameObject(f)
{

}

void PowerMushroom::update(float deltaTime)
{
	Rectangle pos = getPos();
	const float speed = 300;
	const float downSpeed = 500;
	if (left)
	{
		if (!isObjectAt({ pos.x - speed * deltaTime,pos.y,pos.width,pos.height - 3 }, ObjectType::Block))
			pos.x -= speed * deltaTime;
		else
			left = false;
	}
	else
	{
		if (!isObjectAt({ pos.x + speed * deltaTime,pos.y,pos.width,pos.height - 3 }, ObjectType::Block))
			pos.x += speed * deltaTime;
		else
			left = true;
	}
	if (!isObjectAt({ pos.x + 3,pos.y + pos.height,pos.width - 6,downSpeed * deltaTime }, ObjectType::Block))
	{
		pos.y += downSpeed * deltaTime;
	}
	if (isObjectAt(pos, ObjectType::Lava))
	{
		game->removeObj(this);
		game->addToDelete(this);
	}
	moveTo(pos.x, pos.y);
	updatePos();
}