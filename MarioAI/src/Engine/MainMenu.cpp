#include "MainMenu.h"
#include "raylib.h"
#include "Engine.h"
#include "Game.h"
#include "MapEdytor.h"
#include "GameLearnScene.h"

Rectangle startGame = { 100,100,350,64 };
Rectangle mapEdytor = { 100,200,350,64 };

MainMenu::MainMenu()
{
	SetExitKey(KEY_ESCAPE);
}

void MainMenu::update(float deltaTIme)
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		if (CheckCollisionPointRec(GetMousePosition(), startGame))
			Engine::getEngine()->setScene(new GameLearnScene());
		if (CheckCollisionPointRec(GetMousePosition(), mapEdytor))
			Engine::getEngine()->setScene(new MapEdytor());
	}

}

void MainMenu::draw()
{
	DrawRectangleRec(startGame, RED);
	DrawRectangleRec(mapEdytor, RED);
	DrawText("START GAME", startGame.x, startGame.y, 48, BLACK);
	DrawText("MAP EDYTOR", mapEdytor.x, mapEdytor.y, 48, BLACK);
}