#include "Coopa.h"

Coopa::Coopa(Rectangle pos, std::string path, Game* game) :GameObject(pos, path, game)
{

}


Coopa::Coopa(Coopa& g) :GameObject(g)
{

}

Coopa::Coopa(nlohmann::json toRead) :GameObject(toRead)
{

}

void Coopa::update(float deltaTime)
{
	timer += deltaTime * 10;
	if (!alive)
	{
		if (isShellMoving)
		{
			moveShell(deltaTime);
			hitAllObjects();
			playerHit();
		}
		else
		{
			lifeTime -= deltaTime;
			if (lifeTime <= 0)
			{
				alive = true;
			}
			else if(lifeTime <= 0.5f)
			{
				sprite = 4;
			}
		}
	}
	else
	{
		if (haveWings)
		{
			sprite = (int)(timer / 2) % 2;
		}
		else
		{
			sprite = (int)(timer / 2) % 2 + 2;
		}

		moveNormal(deltaTime);
		playerHit();
	}


	updatePos();
}

void Coopa::moveShell(float deltaTime)
{
	Rectangle pos = getPos();
	const float speed = 500;
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
	if (!isObjectAt({ pos.x,pos.y + pos.height,pos.width,deltaTime * fallSpeed }, ObjectType::Block))
	{
		pos.y += deltaTime * fallSpeed;
	}
	moveTo(pos.x, pos.y);
}

void Coopa::playerHit()
{
	std::list<GameObject*> objs = getObjectsAt({pos.x,pos.y+pos.height/3,pos.width,pos.height*2/3}, ObjectType::Player);
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
}
void Coopa::hitAllObjects()
{
	std::list<GameObject*> objs = getObjectsAt(pos, ObjectType::Enemy);
	if (objs.size() > 0)
	{
		for (auto o : objs)
		{
			Hitable* h = dynamic_cast<Hitable*>(o);
			if (!h)
				continue;
			h->hitObj();
			Coopa* c = dynamic_cast<Coopa*>(o);
			if (!c)
				continue;
			c->deleteObject();
		}
	}
}
void Coopa::moveNormal(float deltaTime)
{
	Rectangle pos = getPos();
	const float speed = 160;
	const float fallSpeed = 300;
	if (left)
	{

		if (isObjectAt({ pos.x - deltaTime * speed,pos.y,pos.width,pos.height - 1 }, ObjectType::Block) ||
			isObjectAt({ pos.x - deltaTime * speed,pos.y,pos.width,pos.height - 1 }, ObjectType::Enemy))
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
		if (isObjectAt({ pos.x + deltaTime * speed,pos.y,pos.width,pos.height - 1 }, ObjectType::Block) ||
			isObjectAt({ pos.x + deltaTime * speed,pos.y,pos.width,pos.height - 1 }, ObjectType::Enemy))
		{
			left = true;
		}
		else
		{
			pos.x += deltaTime * speed;
		}
	}
	if (!isObjectAt({ pos.x,pos.y + pos.height,pos.width,deltaTime * fallSpeed }, ObjectType::Block))
	{
		pos.y += deltaTime * fallSpeed;
	}
	moveTo(pos.x, pos.y);
}

void Coopa::draw()
{
	Texture2D texture = getTexture().texture;
	Rectangle pos = getPos();
	Rectangle spritePos = { texture.height * (int)sprite +10,0,texture.height-20,texture.height };
	if (left)
		spritePos.width *= -1;
	pos.y -= pos.height / 2;
	pos.height *= 1.5;

	DrawTexturePro(texture, spritePos, pos, { 0,0 }, 0, WHITE);
}

bool Coopa::hitObj()
{
	if (alive)
	{
		alive = false;
		lifeTime = 1;
		isShellMoving = false;
		sprite = 5;

	}
	else
	{
		if (isShellMoving)
		{
			lifeTime = 1;
		}
		isShellMoving = !isShellMoving;
		sprite = 5;
	}
	return true;
}