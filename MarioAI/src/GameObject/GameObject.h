#pragma once
#include "raylib.h"
#include <list>
#include <string>

enum class ObjectType
{
	NON=0,Block=1,Enemy=2,Bullet=3,Palyer=4,Lava=5,PowerUp=6
};

struct TextureSource
{
	Texture2D texture;
	std::string path;
};
class Game;

class GameObject
{
	Rectangle pos;
	TextureSource texture;
	static std::list<TextureSource> texturesLoaded;
protected:
	Game* game;
public:
	GameObject(Rectangle pos,std::string path,Game* game);

	GameObject(GameObject& o);

	virtual ~GameObject(){}

	virtual void update(float deltaTime) = 0;

	virtual ObjectType getType() = 0;

	virtual void draw();

	virtual GameObject* clone() = 0;

	Rectangle getPos() { return pos; }

	void setGame(Game* game) { this->game = game; }

	void moveTo(float x, float y) { pos.x = x, pos.y = y; }

	std::list<GameObject*> getObjectsAt(Rectangle pos, ObjectType type);

	bool isObjectAt(Rectangle pos, ObjectType type);

	TextureSource getTexture() { return texture; }

	void updatePos();

	friend class Game;
};

