#include "Lava.h"

Lava::Lava(Rectangle pos, std::string path):GameObject(pos,path)
{

}

Lava::Lava(Lava& l):GameObject(l)
{

}

void Lava::update(float deltaTime)
{

}
