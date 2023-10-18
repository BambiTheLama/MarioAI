#include "Player.h"
#include "Game.h"
#include "Block.h"
#include "ObjType/Destoryable.h"

Player::Player(Rectangle pos, std::string path,Game *g):GameObject(pos,path,g)
{
}

Player::Player(Player& m) :GameObject(m)
{

}

void Player::draw()
{
	Texture2D texture = getTexture().texture;
	Rectangle texturePos = { 19*sprite,0,19,texture.height };
	Rectangle pos = getPos();
	DrawTexturePro(texture, texturePos, pos, { 0,0 }, 0, WHITE);
}

void Player::update(float deltaTime)
{
	if (invisibleFrames > 0)
		invisibleFrames -= deltaTime;
	deltaTime *= 21.37;
	Rectangle pos = getPos();
	if (IsKeyDown(KEY_A))
	{
		if (!isObjectAt({ pos.x - speed * deltaTime,pos.y,pos.width,pos.height-1  }, ObjectType::Block))
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
	if (IsKeyPressed(KEY_LEFT))
	{
		sprite--;
	}
	if (IsKeyPressed(KEY_RIGHT))
	{
		sprite++;
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
		if (isObjectAt({ pos.x+3,pos.y + pos.height,pos.width-6,2 }, ObjectType::Block))
		{
			pressJumpTime = pressJumpTimeMax;

		}
		else if (!jumping)
		{
			pos.y += deltaTime / pressJumpTimeMax * jumpHeight;
			pressJumpTime = 0;
		}
		std::list<GameObject*> obj = getObjectsAt({ pos.x+3,pos.y - 1,pos.width-6,1 },ObjectType::Block);
		for (auto o : obj)
		{
			Destoryable* d = dynamic_cast<Destoryable*>(o);
			if (!d)
			{
				pressJumpTime = 0;
				continue;
			}
			d->destory();
		}

	}
	moveTo(pos.x, pos.y);
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