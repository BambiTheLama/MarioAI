#include "Flag.h"

Flag::Flag(Rectangle pos, std::string path, Game* game):GameObject(pos,path,game)
{

}

Flag::Flag(Flag& f) :GameObject(f)
{

}

void Flag::update(float deltaTime)
{

}

void Flag::draw()
{
	Rectangle pos = getPos();
	DrawRectangleRec(pos, RED);
	DrawRectangle(pos.x, pos.y, 3, pos.height, WHITE);
	Texture2D texture = getTexture().texture;
	Rectangle texturePos = { 0,0,-texture.width,texture.height };
	DrawTexturePro(texture, texturePos, { pos.x,pos.y,pos.width,pos.width }, { 0,0 }, 0, WHITE);
}