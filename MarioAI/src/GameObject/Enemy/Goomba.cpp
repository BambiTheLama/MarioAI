#include "Goomba.h"
#include "../ObjType/Hitable.h"

Goomba::Goomba(Rectangle pos, std::string path, Game* game) :GameObject(pos, path, game)
{

}


Goomba::Goomba(Goomba& g) :GameObject(g)
{

}

Goomba::Goomba(nlohmann::json toRead) :GameObject(toRead)
{

}

void Goomba::update(float deltaTime)
{
	Rectangle pos = getPos();
	const float speed = 100;
	const float fallSpeed = 300;
	if (left)
	{

		if (isObjectAt({ pos.x - deltaTime * speed,pos.y,pos.width,pos.height - 1 }, ObjectType::Block))
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
		if (isObjectAt({ pos.x + deltaTime * speed,pos.y,pos.width,pos.height - 1 }, ObjectType::Block))
		{
			left = true;
		}
		else
		{
			pos.x += deltaTime * speed;
		}
	}
	if (!isObjectAt({ pos.x,pos.y+pos.height,pos.width,deltaTime*fallSpeed }, ObjectType::Block))
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
	Rectangle spritePos = { texture.height * ((int)sprite % 2),0,texture.height,texture.height };
	DrawTexturePro(texture, spritePos, pos, { 0,0 }, 0, WHITE);
}