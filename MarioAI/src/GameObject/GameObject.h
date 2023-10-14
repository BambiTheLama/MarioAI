#pragma once
#include "raylib.h"
#include <list>
#include <string>
enum class ObjectType
{
	NON=0,Block=1,Enemy=2,Bullet=3,Palyer=4
};

struct textureSource
{
	Texture2D texture;
	std::string path;
};

class GameObject
{
	Rectangle pos;
	textureSource texture;
	static std::list<textureSource> texturesLoaded;
public:
	GameObject(Rectangle pos,std::string path);

	virtual ~GameObject(){}

	virtual void update() = 0;

	virtual ObjectType getType() = 0;

	virtual void draw();

	friend int main();
};

