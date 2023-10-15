#pragma once
#include "raylib.h"
#include <list>
#include <string>
enum class ObjectType
{
	NON=0,Block=1,Enemy=2,Bullet=3,Palyer=4,Lava=5,
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

	GameObject(GameObject& o);

	virtual ~GameObject(){}

	virtual void update(float deltaTime) = 0;

	virtual ObjectType getType() = 0;

	virtual void draw();

	virtual GameObject* clone() = 0;

	Rectangle getPos() { return pos; }

	void moveTo(int x, int y) { pos.x = x, pos.y = y; }

	friend class Game;
};

