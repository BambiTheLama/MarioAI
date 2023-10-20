#include "PowerFlower.h"

PowerFlower::PowerFlower(Rectangle pos, std::string path, Game* game):GameObject(pos,path,game)
{

}

PowerFlower::PowerFlower(PowerFlower& f):GameObject(f)
{

}

void PowerFlower::update(float deltaTime)
{

}