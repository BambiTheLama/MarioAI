#include "Player.h"
#include "../Engine/Game.h"
#include "Blocks/Block.h"
#include "ObjType/Destoryable.h"
#include "ObjType/PowerType.h"
#include "Flame.h"

Player::Player(Rectangle pos,Game *g,bool AI):GameObject(pos, "res/CzesiekSmall.png",g)
{
	bigPlayer = loadTexture("res/CzesiekBig.png");
	powerPlayer = loadTexture("res/CzesiekExtra.png");
	nn = new NN();
	this->AI = AI;
}

Player::Player(Rectangle pos, Game* g,NN* n) :GameObject(pos, "res/CzesiekSmall.png", g)
{
	bigPlayer = loadTexture("res/CzesiekBig.png");
	powerPlayer = loadTexture("res/CzesiekExtra.png");
	nn = n;
}

Player::Player(Player& m) :GameObject(m)
{
	bigPlayer.texture = m.bigPlayer.texture;
	bigPlayer.path = m.bigPlayer.path;
	powerPlayer.texture = m.powerPlayer.texture;
	powerPlayer.path = m.powerPlayer.path;
	nn = new NN(*m.nn);
}
Player::~Player()
{
	delete nn;
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
	nn->draw(0, 60);
	DrawText(TextFormat("Fitnes: %lf\nTimer: %lf", fitnes, endProcesTimer), 0, 0, 20, BLACK);
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
		endProcesTimer -= deltaTime;
		if (endProcesTimer <= 0)
			game->lostGame();
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
	int startX = (pos.x+pos.width) -  blockSize*2;
	int startY = (pos.y+pos.height) - inputsSizeH *(2.5f/ 4.0f) * blockSize ;
	for (int x = 0; x < inputsSizeW; x++)
		for (int y = 0; y < inputsSizeH; y++)
		{
			std::list<GameObject*> objs = getObjectsAt({ (float)startX+x* blockSize,(float)startY+ blockSize*y,1.0f,1.0f });
			inputs[x + y * inputsSizeW] = 0;
			for (auto o : objs)
			{
				if (o->getType() == ObjectType::Block)
				{
					inputs[x + y * inputsSizeW] = 1;
					break;
				}
				if (o->getType() == ObjectType::Bullet || o->getType() == ObjectType::Lava || o->getType() == ObjectType::Enemy)
				{
					inputs[x + y * inputsSizeW] = -1;
					break;
				}
				if (o->getType() == ObjectType::PowerUp)
				{
					inputs[x + y * inputsSizeW] = 2;
					break;
				}
			}

		}
	nn->setInputs(inputs);
	if (IsKeyPressed(KEY_UP))
		nn->mutate();

	nn->generateOutput();

	bool* outputs = nn->getOutputs();

	if (outputs[1])
	{
		pressD();
	}
	else if (outputs[0])
	{
		pressA();
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
	if (outputs[4] || jumpFromEnemy || (pressJumpTime > pressJumpTimeMax - 2 && pressJumpTime != pressJumpTimeMax))
	{
		pressSpace();
	}
	else
	{

		jumping = false;
		if (pressJumpTime != pressJumpTimeMax)
			pressJumpTime = 0;
	}
	float newFitnes = pos.x + points*10;
	if (newFitnes > fitnes)
	{
		fitnes = newFitnes;
		endProcesTimer = endProcesTimerMax;
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
	if (IsKeyDown(KEY_SPACE) || jumpFromEnemy || (pressJumpTime > pressJumpTimeMax - 2 && pressJumpTime != pressJumpTimeMax))
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
			const float fallSpeed = 40;

			if (!isObjectAt({ pos.x + 3,pos.y + pos.height,pos.width - 6,deltaTime * fallSpeed }, ObjectType::Block))
			{
				pos.y += deltaTime * fallSpeed;
				pressJumpTime = 0;
			}
			else
			{
				float t = 1.0f;
				while (t>0.01f)
				{
					if (!isObjectAt({ pos.x + 3,pos.y + pos.height,pos.width - 6,deltaTime * fallSpeed*t }, ObjectType::Block))
					{
						pos.y += deltaTime * fallSpeed*t;
						pressJumpTime = 0;
						break;
					}
					t /= 2;
				}
				pressJumpTime = pressJumpTimeMax;
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
			fitnes = 9999999;
		}
		obj = getObjectsAt({ pos.x,pos.y + pos.height+1,pos.width,4 }, ObjectType::Enemy);
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
void Player::setNeuronNetwork(NN* nn)
{
	if (nn)
	{
		if (this->nn)
			delete nn;
		this->nn = nn;
	}
}
