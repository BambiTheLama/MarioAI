#pragma once
#include "GameObject.h"

class Particle
{
    TextureSource texture;
    Rectangle pos;
    Rectangle texturePos;
    float rotation = 0;
    bool rotationLeft = false;
    float lifeTime = 1;
    float speed = 20;
public:
    Particle(TextureSource texture, Rectangle texturePos, Rectangle pos, float lifeTime = 1);
    void update(float deltaTime);
    void draw();
    friend class ParticleSystem;
};

class ParticleSystem :
    public GameObject
{
    int size;
    Particle** particles;
public:
    ParticleSystem(Rectangle pos, std::string path, Game* game);

    ParticleSystem(ParticleSystem& p);

    ~ParticleSystem();

    void update(float deltaTime);

    void draw();

    ObjectType getType() { return ObjectType::NON; }

    ParticleSystem* clone() { return new ParticleSystem(*this); }
};

