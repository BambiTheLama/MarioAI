#include "MainMenu.h"
#include "raylib.h"
#include "Engine.h"
#include "Game.h"
#include "MapEdytor.h"
#include "GameLearnScene.h"

Rectangle AIGame = { 100,100,350,64 };
Rectangle startGame = { 100,200,350,64 };
Rectangle mapEdytor = { 100,300,350,64 };

MainMenu::MainMenu()
{
	SetExitKey(KEY_ESCAPE);
}

void MainMenu::update(float deltaTIme)
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		if (CheckCollisionPointRec(GetMousePosition(), AIGame))
			Engine::getEngine()->setScene(new GameLearnScene());
		if (CheckCollisionPointRec(GetMousePosition(), startGame))
			Engine::getEngine()->setScene(new Game(false));
		if (CheckCollisionPointRec(GetMousePosition(), mapEdytor))
			Engine::getEngine()->setScene(new MapEdytor());
	}

}

void MainMenu::draw()
{
	DrawRectangleRec(AIGame, RED);
	DrawRectangleRec(startGame, RED);
	DrawRectangleRec(mapEdytor, RED);
	DrawText("AI GAME", AIGame.x, AIGame.y, 48, BLACK);
	DrawText("START GAME", startGame.x, startGame.y, 48, BLACK);
	DrawText("MAP EDYTOR", mapEdytor.x, mapEdytor.y, 48, BLACK);
}