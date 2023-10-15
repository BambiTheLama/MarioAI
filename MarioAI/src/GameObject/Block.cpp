#include "Block.h"

Block::Block(Rectangle pos,std::string path,Game* game) :GameObject(pos,path,game)
{

}
Block::Block(Block& b) :GameObject(b)
{

}
void Block::update(float deltaTime)
{

}
