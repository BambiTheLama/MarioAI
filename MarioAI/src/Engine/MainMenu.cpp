#include "MainMenu.h"
#include "raylib.h"
#include "Engine.h"
#include "Game.h"
#include "MapEditor.h"
#include "GameLearnScene.h"

Rectangle AIGame;
Rectangle startGame;
Rectangle mapEditor;
Texture2D background;
Color buttonColor;

MainMenu::MainMenu() {
  SetExitKey(KEY_ESCAPE);
  background = LoadTexture("res/background.png");
  buttonColor = GetColor(0x2850ffff);

  // Wyliczenie œrodkowych pozycji przycisków
  const int screenWidth = GetScreenWidth();
  const int screenHeight = GetScreenHeight();
  const int buttonWidth = 350;
  const int buttonHeight = 100;
  const int buttonSpacing = 20;

  const int buttonX = (screenWidth - buttonWidth) / 2;
  const int aiGameButtonY = screenHeight / 3 - (buttonHeight + buttonSpacing);
  const int startGameButtonY = screenHeight / 3;
  const int mapEditorButtonY = screenHeight / 3 + (buttonHeight + buttonSpacing);

  AIGame = { static_cast<float>(buttonX), static_cast<float>(aiGameButtonY), static_cast<float>(buttonWidth), static_cast<float>(buttonHeight) };
  startGame = { static_cast<float>(buttonX), static_cast<float>(startGameButtonY), static_cast<float>(buttonWidth), static_cast<float>(buttonHeight) };
  mapEditor = { static_cast<float>(buttonX), static_cast<float>(mapEditorButtonY), static_cast<float>(buttonWidth), static_cast<float>(buttonHeight) };
}
MainMenu::~MainMenu()
{
    UnloadTexture(background);
}

void MainMenu::update(float deltaTime) 
{
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    if (CheckCollisionPointRec(GetMousePosition(), AIGame))
      Engine::getEngine()->setScene(new GameLearnScene());
    if (CheckCollisionPointRec(GetMousePosition(), startGame)) {
      SetExitKey(0);
      Engine::getEngine()->setScene(new Game(false));
    }
    if (CheckCollisionPointRec(GetMousePosition(), mapEditor))
      Engine::getEngine()->setScene(new MapEditor());
  }
}

void MainMenu::draw() 
{

  DrawTexture(background, 0, 0, WHITE);

  // Rysowanie zaokr¹glonych przycisków
  DrawRectangleRounded(AIGame, 1.f, 0, buttonColor);
  DrawRectangleRounded(startGame, 1.f, 0, buttonColor);
  DrawRectangleRounded(mapEditor, 1.f, 0, buttonColor);

  // Tekst na przyciskach
  DrawTextEx(CustomFont::customFont, "AI GAME", Vector2{ AIGame.x + AIGame.width / 2 - MeasureTextEx(CustomFont::customFont, "AI GAME", 64, 2).x / 2, AIGame.y + AIGame.height / 2 - 32 }, 64, 2, BLACK);
  DrawTextEx(CustomFont::customFont, "START GAME", Vector2{ startGame.x + startGame.width / 2 - MeasureTextEx(CustomFont::customFont, "START GAME", 64, 2).x / 2, startGame.y + startGame.height / 2 - 32 }, 64, 2, BLACK);
  DrawTextEx(CustomFont::customFont, "MAP EDITOR", Vector2{ mapEditor.x + mapEditor.width / 2 - MeasureTextEx(CustomFont::customFont, "MAP EDITOR", 64, 2).x / 2, mapEditor.y + mapEditor.height / 2 - 32 }, 64, 2, BLACK);



}
