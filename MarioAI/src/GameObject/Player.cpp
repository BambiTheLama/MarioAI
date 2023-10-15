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
		pos.x -= speed * deltaTime;
	}
	if (IsKeyDown(KEY_D))
	{
		pos.x += speed * deltaTime;
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
		Rectangle objsPos = { pos.x,pos.y + pos.height,pos.width,2 };
		std::list<GameObject*> obj = game->getObjectsAt(objsPos);
		for (auto o : obj)
		{
			if(CheckCollisionRecs(o->getPos(), objsPos))
			if (o->getType() == ObjectType::Block)
			{
				pressJumpTime = pressJumpTimeMax;
			}
		}

		if (!jumping && obj.size() <= 0)
		{
			pos.y += deltaTime / pressJumpTimeMax * jumpHeight;
			pressJumpTime = 0;
		}

		obj = game->getObjectsAt({ pos.x,pos.y -1,pos.width,1 });
		for (auto o : obj)
		{
			if (o->getType() == ObjectType::Block)
			{
				pressJumpTime = 0;
			}
		}

	}
	moveTo(pos.x, pos.y);
}