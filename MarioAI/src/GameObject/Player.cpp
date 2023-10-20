#include "Player.h"
#include "../Engine/Game.h"
#include "Blocks/Block.h"
#include "ObjType/Destoryable.h"
#include "ObjType/PowerType.h"
Player::Player(Rectangle pos,Game *g):GameObject(pos, "res/CzesiekSmall.png",g)
{
	bigPlayer.texture = LoadTexture("res/CzesiekBig.png");
	bigPlayer.path = "res/CzesiekBig.png";
	powerPlayer.texture = LoadTexture("res/CzesiekExtra.png");;
	powerPlayer.path = "res/CzesiekExtra.png";
}

Player::Player(Player& m) :GameObject(m)
{

}

void Player::draw()
{
	Texture2D texture = getTexture().texture;
	switch (hp)
	{
		case 1:
			texture = getTexture().texture;
			break;
		case 2:
			texture = bigPlayer.texture;
			break;
		case 3:
			texture = powerPlayer.texture;
			break;
		default:
			texture = getTexture().texture;

	}
	Rectangle texturePos;
	if(hp>1)
		texturePos = { (float)texture.height *sprite+ texture.height / 4,0,(float)texture.height/2,(float)texture.height };
	else
		texturePos = { (float)texture.height * sprite + texture.height / 4,(float)texture.height * 1.0f / 3.0f,(float)texture.height / 2,(float)texture.height*2.0f/3.0f };
	if (moveLeft)
		texturePos.width *= -1;
	Rectangle pos = getPos();
	//DrawRectangleRec(pos, RED);
	DrawTexturePro(texture, texturePos, pos, { 0,0 }, 0, WHITE);
}

void Player::update(float deltaTime)
{
	if (invisibleFrames > 0)
		invisibleFrames -= deltaTime;
	deltaTime *= 21.37;
	Rectangle pos = getPos();

	if (IsKeyDown(KEY_D))
	{
		if (!isObjectAt({ pos.x + speed * deltaTime,pos.y,pos.width,pos.height - 1 }, ObjectType::Block))
		{
			pos.x += speed * deltaTime;
		}
		moveLeft = false;
		distance += deltaTime / 2;
		sprite = 1 + (int)distance % 5;
		if (hp > 1)
			sprite++;
	}
	else if(IsKeyDown(KEY_A))
	{
		if (!isObjectAt({ pos.x - speed * deltaTime,pos.y,pos.width,pos.height - 1 }, ObjectType::Block))
		{
			pos.x -= speed * deltaTime;
		}
		moveLeft = true;
		distance += deltaTime / 2;
		sprite = 1 + (int)distance % 5;
		if (hp > 1)
			sprite++;
	}
	else if (IsKeyDown(KEY_S))
	{
		sprite = 0;
		if (hp > 1)
			sprite++;
	}
	else
	{
		distance = 0;
		sprite = 0;
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
	else 
	{
		jumping = false;
		if (pressJumpTime != pressJumpTimeMax)
			pressJumpTime = 0;
	}
	if (game)
	{
		if (isObjectAt({ pos.x+3,pos.y + pos.height,pos.width-6,1 }, ObjectType::Block))
		{
			pressJumpTime = pressJumpTimeMax;

		}
		else if (!jumping)
		{
			float y = pos.y;
			float times = 1;
			do
			{
				pos.y = y;
				pos.y += deltaTime / pressJumpTimeMax * jumpHeight/times;
				times++;
			}
			while (isObjectAt(pos, ObjectType::Block) && times<10);
			if (times >= 10)
			{
				pos.y = y;
				pressJumpTime = pressJumpTimeMax;
			}
			else
			{
				pressJumpTime = 0;
			}

		}
		std::list<GameObject*> obj = getObjectsAt({ pos.x+3,pos.y - 1,pos.width-6,1 },ObjectType::Block);
		for (auto o : obj)
		{
			Destoryable* d = dynamic_cast<Destoryable*>(o);
			if (d)
			{
				d->destory(hp);
			}
			pressJumpTime = 0;
		}
		

	}
	moveTo(pos.x, pos.y);
	checkPowerUps();
}
void Player::checkPowerUps()
{
	std::list<GameObject*> obj = getObjectsAt(getPos(), ObjectType::PowerUp);
	for (auto o : obj)
	{
		PowerTypeObject* type = dynamic_cast<PowerTypeObject*>(o);
		if (!type)
			continue;
		switch (type->getPower())
		{
		case PowerType::Coin:
			points++;
			break;
		case PowerType::Mushroom:
			if (hp < 2)
			{
				hp = 2;
				pos.y -= pos.height;
				pos.height *= 2;
			}
			break;
		case PowerType::Plant:
			if (hp < 2)
			{
				hp = 2;
				pos.y -= pos.height;
				pos.height *= 2;
			}
			else if (hp < 3)
				hp = 3;
			break;
		}
		game->removeObj(o);
		game->addToDelete(o);
	}
}

void Player::hitObj() 
{ 
	if (invisibleFrames <= 0)
	{
		hp--;
		if (hp <= 0 && game)
			game->lostGame();
	}

}