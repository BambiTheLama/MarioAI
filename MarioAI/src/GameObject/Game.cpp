#include "Game.h"


Game::Game()
{
	InitWindow(1600, 900, "MARIO");
}

Game::~Game()
{
	CloseWindow();
}

void Game::draw()
{

}

void Game::update()
{

}


void Game::start()
{
	while (!WindowShouldClose())
	{
		update();
		BeginDrawing();
		ClearBackground(WHITE);
		draw();
		EndDrawing();
	}
}