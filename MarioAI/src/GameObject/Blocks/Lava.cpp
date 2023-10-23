#include "Lava.h"

Lava::Lava(Rectangle pos, std::string path,Game*game):GameObject(pos,path,game)
{

}

Lava::Lava(Lava& l):GameObject(l)
{

}

void Lava::update(float deltaTime)
{

}

void Lava::draw()
{
	DrawRectangleRec(pos, RED);
}
