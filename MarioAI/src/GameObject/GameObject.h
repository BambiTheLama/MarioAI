#pragma once
#include "raylib.h"
#include <list>
#include <string>
#include "../json.hpp"

enum class ObjectType
{
	NON=0,
	Block=1,
	Enemy=2,
	Bullet=3,
	Player=4,
	Lava=5,
	PowerUp=6,
	Finish
};

struct TextureSource
{
	Texture2D texture;
	std::string path;
};
class Game;

class GameObject
{

	TextureSource texture;
	static std::list<TextureSource> texturesLoaded;
protected:
	Game* game;
	Rectangle pos;
public:
	GameObject(Rectangle pos,std::string path,Game* game);

	GameObject(GameObject& o);

	GameObject(nlohmann::json readFile);

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

	void deleteObject();

	virtual void saveToFile(nlohmann::json& saveFile);

	friend class Engine;

};

