#include "ParticleSystem.h"
#include "Game.h"

Particle::Particle(TextureSource texture, Rectangle texturePos, Rectangle pos, float lifeTime)
{
	this->lifeTime = lifeTime;
	this->texturePos = texturePos;
	this->pos = pos;
	this->texture = texture;
}
void Particle::update(float deltaTime)
{
	lifeTime -= deltaTime;
	if (rotationLeft)
		rotation += deltaTime * 40;
	else
		rotation -= deltaTime * 40;
	speed += deltaTime * 50;
	pos.y += speed*deltaTime*10;
	
}
void Particle::draw()
{
	DrawTexturePro(texture.texture, texturePos, pos, { texturePos.width / 2,texturePos.height / 2 }, rotation, WHITE);
}

ParticleSystem::ParticleSystem(Rectangle pos, std::string path, Game* game):GameObject(pos,path,game)
{
	this->size = 9;
	particles = new Particle * [size];
	TextureSource texture = getTexture();
	for (int i = 0; i < size; i++)
	{
		Rectangle posParticle = { pos.x + (pos.width / 3 * (i % 3)),pos.y + ((int)pos.height / 3 * i / 3),pos.width / 3,pos.height / 3 };
		Rectangle texturePos = { 0,0,texture.texture.width / 3,texture.texture.height / 3 };
		texturePos.x = texturePos.width * (i % 3);
		texturePos.y = texturePos.height * (i / 3);
		particles[i] = new Particle(texture, texturePos, posParticle, 2);
		particles[i]->rotationLeft = i % 2;
	}

}

ParticleSystem::ParticleSystem(ParticleSystem& p):GameObject(p)
{
	Rectangle pos = getPos();
	this->size = 9;
	particles = new Particle * [size];
	TextureSource texture = getTexture();
	for (int i = 0; i < size; i++)
	{
		Rectangle posParticle = { pos.x + (pos.width / 3 * (i % 3)),pos.y + ((int)pos.height / 3 * i / 3),pos.width / 3,pos.height / 3 };
		Rectangle texturePos = { 0,0,texture.texture.width / 3,texture.texture.height / 3 };
		texturePos.x = texturePos.width * (i % 3);
		texturePos.y = texturePos.height * (i / 3);
		particles[i] = new Particle(texture, texturePos, posParticle, 5);
		particles[i]->rotationLeft = i % 2;
	}
}
ParticleSystem::~ParticleSystem()
{
	for (int i = 0; i < size; i++)
		delete particles[i];
	delete particles;
}
void ParticleSystem::update(float deltaTime)
{
	for (int i = 0; i < size; i++)
		particles[i]->update(deltaTime);
	if (particles[0]->lifeTime <= 0)
	{
		game->removeObj(this);
		game->addToDelete(this);
	}

		
}

void ParticleSystem::draw()
{
	for (int i = 0; i < size; i++)
		particles[i]->draw();
}