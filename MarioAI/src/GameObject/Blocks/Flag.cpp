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
	DrawRectangle(pos.width/2 + pos.x-7, pos.y, 7, pos.height, GREEN);
	DrawRectangle(pos.width/2 + pos.x-6, pos.y, 5, pos.height, WHITE);
	DrawRectangle(pos.width/2 + pos.x-5, pos.y, 3, pos.height, GREEN);
	Texture2D texture = getTexture().texture;
	Rectangle texturePos = { 0,0,texture.width,texture.height };

	DrawTexturePro(texture, texturePos, { pos.x-pos.width/2,pos.y,pos.width,pos.width }, { 0,0 }, 0, WHITE);
	DrawCircle(pos.width/2 + pos.x - 4, pos.y - 11, 11, BLACK);
	DrawCircle(pos.width/2 + pos.x - 3, pos.y-9, 7, GREEN);
}