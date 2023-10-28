#include "Player.h"
#include "../Engine/Game.h"
#include "Blocks/Block.h"
#include "ObjType/Destoryable.h"
#include "ObjType/PowerType.h"
#include "Flame.h"
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
	if (invisibleFrames <= 0 || (int)(invisibleFrames*10)%10>3)
	{
		DrawTexturePro(texture, texturePos, pos, { 0,0 }, 0, WHITE);
	}


}

void Player::drawInterface()
{
	nn.draw(0, 0);
}

void Player::update(float deltaTime)
{

	this->deltaTime = deltaTime*20;
	if (invisibleFrames > 0)
		invisibleFrames -= deltaTime;
	if (flameCdr > 0)
		flameCdr -= deltaTime;
	if (AI)
	{
		aiControll();
	}
	else
	{
		playerControll();
	}
	
	objectInteration();
	checkPowerUps();
}

void Player::aiControll()
{
	int inputs[inputsSize];

	nn.setInputs(inputs);
	if (IsKeyPressed(KEY_UP))
		nn.mutate();

	nn.generateOutput();

	bool* outputs = nn.getOutputs();

	if (outputs[0])
	{
		pressA();
	}
	else if (outputs[1])
	{
		pressD();
	}
	else if (outputs[2])
	{
		pressS();
	}
	else
	{
		distance = 0;
		sprite = 0;
	}
	if (hp == 3 && flameCdr <= 0 && outputs[3])
	{
		pressControl();
	}
	if (outputs[4] || jumpFromEnemy)
	{
		pressSpace();
	}
	else
	{

		jumping = false;
		if (pressJumpTime != pressJumpTimeMax)
			pressJumpTime = 0;
	}
}

void Player::playerControll()
{
	if (IsKeyDown(KEY_D))
	{
		pressD();
	}
	else if (IsKeyDown(KEY_A))
	{
		pressA();
	}
	else if (IsKeyDown(KEY_S))
	{
		pressS();
	}
	else
	{
		distance = 0;
		sprite = 0;
	}
	if (hp == 3 && flameCdr <= 0 && IsKeyDown(KEY_LEFT_CONTROL))
	{
		pressControl();
	}
	if (IsKeyDown(KEY_SPACE) || jumpFromEnemy)
	{
		pressSpace();
	}
	else
	{
		jumping = false;
		if (pressJumpTime != pressJumpTimeMax)
			pressJumpTime = 0;
	}
}

void Player::objectInteration()
{
	if (game)
	{
		if (isObjectAt({ pos.x + 6,pos.y + pos.height,pos.width - 12,1 }, ObjectType::Block))
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
				pos.y += deltaTime / pressJumpTimeMax * jumpHeight / times;
				times++;
			} while (isObjectAt({ pos.x + 6,pos.y + pos.height,pos.width - 12,1 }, ObjectType::Block) && times < 5);
			if (times >= 5)
			{
				pos.y = y;
				pressJumpTime = pressJumpTimeMax;
			}
			else
			{
				pressJumpTime = 0;
			}

		}
		std::list<GameObject*> obj = getObjectsAt({ pos.x + 6,pos.y - 1,pos.width - 12,1 }, ObjectType::Block);
		for (auto o : obj)
		{
			Destoryable* d = dynamic_cast<Destoryable*>(o);
			if (d)
			{
				d->destory(hp);
			}
			pressJumpTime = 0;
		}
		obj = getObjectsAt(pos, ObjectType::Lava);
		for (auto o : obj)
		{
			game->lostGame();
		}
		obj = getObjectsAt(pos, ObjectType::Finish);
		if (obj.size() > 0)
		{
			game->setWin();
		}
		obj = getObjectsAt({ pos.x,pos.y + pos.height,pos.width,2 }, ObjectType::Enemy);
		if (obj.size() > 0)
		{
			for (auto o : obj)
			{
				Hitable* h = dynamic_cast<Hitable*>(o);
				if (!h)
					continue;
				if (h->hitObj())
				{
					pressJumpTime = 2;
					jumpFromEnemy = true;
				}

			}
		}

	}
}

void Player::pressA()
{
	if (!isObjectAt({ pos.x - speed * deltaTime,pos.y+4,pos.width,pos.height - 8 }, ObjectType::Block))
	{
		pos.x -= speed * deltaTime;
	}
	moveLeft = true;
	distance += deltaTime / 2;
	sprite = 1 + (int)distance % 5;
	if (hp > 1)
		sprite++;
}

void Player::pressD()
{
	if (!isObjectAt({ pos.x + speed * deltaTime,pos.y+4,pos.width,pos.height - 8 }, ObjectType::Block))
	{
		pos.x += speed * deltaTime;
	}
	moveLeft = false;
	distance += deltaTime / 2;
	sprite = 1 + (int)distance % 5;
	if (hp > 1)
		sprite++;
}

void Player::pressSpace()
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
		jumpFromEnemy = false;
	}
	pos.y -= t / pressJumpTimeMax * jumpHeight;
}

void Player::pressS()
{
	sprite = 0;
	if (hp > 1)
		sprite++;
}

void Player::pressControl()
{
	Flame* f = new Flame({ pos.x + pos.width / 2,pos.y + pos.height / 2,32,32 }, "res/Shot.png", game, moveLeft);
	game->addObj(f);
	flameCdr = 1;
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
			game->removeBlock(o);
			break;
		case PowerType::Mushroom:
			game->removeObj(o);
			game->addToDelete(o);
			if (hp < 2)
			{
				hp = 2;
				pos.y -= pos.height;
				pos.height *= 2;
			}
			break;
		case PowerType::Plant:
			game->removeObj(o);
			game->addToDelete(o);
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


	}
}

bool Player::hitObj() 
{ 
	if (invisibleFrames <= 0)
	{
		hp--;
		if (hp == 1)
		{
			pos.height /= 2;
			pos.y += pos.height;
		}
		invisibleFrames = 2;
		if (hp <= 0 && game)
			game->lostGame();
		return true;
	}
	return false;
}