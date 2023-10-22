#include "Flame.h"

Flame::Flame(Rectangle pos, std::string path, Game* game,bool left):GameObject(pos,path,game)
{
	this->left = left;
}

Flame::Flame(Flame& f) :GameObject(f)
{
	this->left = f.left;
}

void Flame::update(float deltaTime)
{
	timer += deltaTime * 10;
	if (hit)
	{
		lifeTime -= deltaTime;
		if (lifeTime > 0.66)
			sprite = 1;
		else if (lifeTime > 0.33)
			sprite = 2;
		else
			sprite = 3;
		if (lifeTime <= 0)
		{
			deleteObject();
			return;
		}
	}
	else
	{
		sprite = (int)timer % 4;
		move(deltaTime);

	}
}

void Flame::move(float deltaTime)
{
	Rectangle pos = getPos();
	const float speed = 500;
	const float fallSpeed = 300;
	if (left)
	{
		if (isObjectAt({ pos.x - deltaTime * speed,pos.y,pos.width,pos.height - 1 }, ObjectType::Block))
		{
			hit = true;
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
			hit = true;
		}
		else
		{
			pos.x += deltaTime * speed;
		}
	}
	if (upPos > 0)
	{
		upPos -= fallSpeed * deltaTime;
		pos.y -= fallSpeed * deltaTime;

	}
	else
	{
		if (!isObjectAt({ pos.x,pos.y + pos.height,pos.width,deltaTime * fallSpeed }, ObjectType::Block))
		{
			pos.y += deltaTime * fallSpeed;
		}
		else
		{
			upPos = 100;
		}

	}
	moveTo(pos.x, pos.y);
	updatePos();
	std::list<GameObject*> objs = getObjectsAt(pos, ObjectType::Enemy);
	if (objs.size() > 0)
	{
		for (auto o : objs)
		{
			o->deleteObject();
		}
	}
}

void Flame::draw()
{
	Texture2D texture = getTexture().texture;
	Rectangle texturePos;
	Rectangle pos = getPos();
	if (hit)
	{
		texturePos = { (float)texture.height * sprite,0,(float)texture.height,(float)texture.height };
	}
	else
	{
		texturePos = { texture.height / 2.0f * (sprite % 2),texture.height / 2.0f * (sprite / 2),
			(float)texture.height / 2,(float)texture.height / 2 };
	}

	DrawTexturePro(texture, texturePos, pos, { 0,0 }, 0, WHITE);
}