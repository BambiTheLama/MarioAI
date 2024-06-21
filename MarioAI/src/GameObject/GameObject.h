#pragma once
#include "raylib.h"
#include <list>
#include <string>
#include "../json.hpp"


struct CustomFont {
	static Font customFont;
	static void loadFont()
	{
		customFont = LoadFont("res/PatrickHand.ttf");
	}
	static void unloadFont()
	{
		UnloadFont(customFont);
	}
};

enum class ObjectType
{
	NON=0,
	Block=1,
	Enemy=2,
	Bullet=3,
	Player=4,
	Lava=5,
	PowerUp=6,
	Finish,
	PlayerProjectal,
};

struct TextureSource
{
	Texture2D texture;
	std::string path;
};
class Game;

class GameObject
{
	int ID = 0;
	TextureSource texture;
	static std::list<TextureSource> texturesLoaded;
protected:
	Game* game;
	Rectangle pos;
public:
	GameObject(Rectangle pos,std::string path,Game* game);

	GameObject(GameObject& o);

	virtual ~GameObject();

	virtual void update(float deltaTime) = 0;

	virtual ObjectType getType() = 0;

	virtual void draw();

	virtual void readFromFile(nlohmann::json& readFile);

	virtual GameObject* clone() = 0;

	Rectangle getPos() { return pos; }

	void setGame(Game* game) { this->game = game; }

	void moveTo(float x, float y) { pos.x = x, pos.y = y; }

	std::list<GameObject*> getObjectsAt(Rectangle pos, ObjectType type);

	std::list<GameObject*> getObjectsAt(Rectangle pos);

	bool isObjectAt(Rectangle pos, ObjectType type);

	TextureSource getTexture() { return texture; }

	void updatePos();

	void deleteObject();

	virtual void saveToFile(nlohmann::json& saveFile);

	void setPos(Rectangle pos) { this->pos = pos; }

	int getID() { return ID; }

	TextureSource loadTexture(std::string path);

	friend class Engine;
	friend void createObjectToCopy();

};

