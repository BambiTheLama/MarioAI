#include "DestroyableBlock.h"
#include "../../Engine/Game.h"
#include "../ParticleSystem.h"
DestroyableBlock::DestroyableBlock(Rectangle pos, std::string path, Game* game):Block(pos,path,game)
{

}

DestroyableBlock::DestroyableBlock(DestroyableBlock& d):Block(d)
{

}

void DestroyableBlock::destroy(int power)
{
	if (power > 1 && game)
	{
		ParticleSystem* particleSystem = new ParticleSystem(getPos(), getTexture().path, game);
		game->addObj(particleSystem);
		game->removeBlock(this);
	}
}