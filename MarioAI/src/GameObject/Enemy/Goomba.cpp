#include "Goomba.h"
#include "../ObjType/Hitable.h"

Goomba::Goomba(Rectangle pos, std::string path, Game* game) :GameObject(pos, path, game)
{

}


Goomba::Goomba(Goomba& g) :GameObject(g)
{

}

void Goomba::update(float deltaTime)
{
	if (!alive)
	{
		lifeTime -= deltaTime;
		if (lifeTime <= 0)
		{
			deleteObject();
		}
		return;
	}
	sprite += deltaTime * 10;
	sprite -= (int)sprite / 2 * 2;
	Rectangle pos = getPos();
	const float speed = 160;
	const float fallSpeed = 300;
	if (left)
	{

		if (isObjectAt({ pos.x - deltaTime * speed,pos.y-3,pos.width,pos.height - 5 }, ObjectType::Block) ||
			isObjectAt({ pos.x - deltaTime * speed,pos.y-3,pos.width,pos.height - 5 }, ObjectType::Enemy))
		{
			left = false;
		}
		else
		{
			pos.x -= deltaTime * speed;
		}
	}
	else
	{
		if (isObjectAt({ pos.x + deltaTime * speed,pos.y-3,pos.width,pos.height - 5 }, ObjectType::Block) || 
			isObjectAt({ pos.x + deltaTime * speed,pos.y-3,pos.width,pos.height - 5 }, ObjectType::Enemy))
		{
			left = true;
		}
		else
		{
			pos.x += deltaTime * speed;
		}
	}
	if (!isObjectAt({ pos.x + 3,pos.y + pos.height,pos.width - 6,deltaTime * fallSpeed }, ObjectType::Block))
	{
		pos.y += deltaTime * fallSpeed;
	}
	std::list<GameObject*> objs = getObjectsAt(pos, ObjectType::Player);
	if (objs.size() > 0)
	{
		for (auto o : objs)
		{
			Hitable* h = dynamic_cast<Hitable*>(o);
			if (!h)
				continue;
			h->hitObj();
		}
	}
	moveTo(pos.x, pos.y);
	updatePos();
}

void Goomba::draw()
{
	Texture2D texture = getTexture().texture;
	Rectangle pos = getPos();
	Rectangle spritePos = { texture.height * (int)sprite,0,texture.height,texture.height };
	DrawTexturePro(texture, spritePos, pos, { 0,0 }, 0, WHITE);
}

bool Goomba::hitObj()
{
	if (alive)
	{
		alive = false;
		sprite = 2;
		return true;
	}
	return false;

}