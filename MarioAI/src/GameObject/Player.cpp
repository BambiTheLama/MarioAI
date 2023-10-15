#include "Player.h"
#include "Game.h"

Player::Player(Rectangle pos, std::string path,Game *g):GameObject(pos,path)
{
	this->game = g;
}

Player::Player(Player& m) :GameObject(m)
{

}

void Player::draw()
{
	DrawRectangleRec(getPos(), RED);
}

void Player::update(float deltaTime)
{
	deltaTime *= 21.37;
	Rectangle pos = getPos();
	if (IsKeyDown(KEY_A))
	{
		if (!isObjectAt({ pos.x - speed * deltaTime,pos.y,pos.width,pos.height-1 }, ObjectType::Block))
		{
			pos.x -= speed * deltaTime;
		}
	}
	if (IsKeyDown(KEY_D))
	{
		if (!isObjectAt({ pos.x + speed * deltaTime,pos.y,pos.width,pos.height-1 }, ObjectType::Block))
		{
			pos.x += speed * deltaTime;
		}

	}
	if (IsKeyDown(KEY_SPACE))
	{
		float t = 0;
		jumping = true;
		if (deltaTime < pressJumpTime)
		{
			pressJumpTime -= deltaTime;
			t = deltaTime;
		}
		else
		{
			t = pressJumpTime;
			pressJumpTime = 0;
			jumping = false;
		}
		pos.y -= t /pressJumpTimeMax*jumpHeight;
		
	}
	else if (pressJumpTime != pressJumpTimeMax)
	{
		jumping = false;
		pressJumpTime = 0;
	}
	if (game)
	{
		if (isObjectAt({ pos.x,pos.y + pos.height,pos.width,2 }, ObjectType::Block))
		{
			pressJumpTime = pressJumpTimeMax;
		}
		else if (!jumping)
		{
			pos.y += deltaTime / pressJumpTimeMax * jumpHeight;
			pressJumpTime = 0;
		}
		if (isObjectAt({ pos.x,pos.y - 1,pos.width,1 }, ObjectType::Block))
		{
			pressJumpTime = 0;
		}

	}
	moveTo(pos.x, pos.y);
}

bool Player::isObjectAt(Rectangle pos,ObjectType type)
{
	std::list<GameObject*> obj = game->getObjectsAt(pos);
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