#include "DestoryAbleBlock.h"
#include "Game.h"
DestoryAbleBlock::DestoryAbleBlock(Rectangle pos, std::string path, Game* game):Block(pos,path,game)
{

}

DestoryAbleBlock::DestoryAbleBlock(DestoryAbleBlock& d):Block(d)
{

}

void DestoryAbleBlock::destory()
{
	if (game)
	{
		game->removeBlock(this);
		game->addToDelete(this);
	}
}