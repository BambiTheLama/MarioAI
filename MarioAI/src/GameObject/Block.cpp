#include "Block.h"

Block::Block(Rectangle pos,std::string path) :GameObject(pos,path)
{

}
Block::Block(Block& b) :GameObject(b)
{

}
void Block::update(float deltaTime)
{

}
