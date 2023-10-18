#include "DestoryAbleBlock.h"
#include "Game.h"
#include "ParticleSystem.h"
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
		ParticleSystem* particleSystem = new ParticleSystem(getPos(), getTexture().path, game);
		game->addObj(particleSystem);
		game->removeBlock(this);
		game->addToDelete(this);


	}
}