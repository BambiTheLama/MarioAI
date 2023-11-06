#include "MainMenu.h"
#include "raylib.h"
#include "Engine.h"
#include "Game.h"
#include "MapEditor.h"
#include "GameLearnScene.h"

Button::Button(Rectangle pos, std::string text)
{
    this->pos = pos;
    this->text = text;
}

void Button::draw()
{
    DrawRectangleRounded(pos, 1.f, 0, GetColor(0x2850ffff));
    const char* t = text.c_str();
    DrawTextEx(CustomFont::customFont, t, Vector2{ pos.x + pos.width / 2 - MeasureTextEx(CustomFont::customFont, t, 64, 2).x / 2, pos.y + pos.height / 2 - 32 }, 64, 2, BLACK);
}

bool Button::press()
{
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), pos);
}

MainMenu::MainMenu() {
    SetExitKey(KEY_ESCAPE);
    background = LoadTexture("res/background.png");
    
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
    
    buttons[0] = new Button({ (float)buttonX, (float)aiGameButtonY, (float)buttonWidth, (float)buttonHeight }, "AI GAME");
    buttons[1] = new Button({ (float)buttonX, (float)startGameButtonY, (float)buttonWidth, (float)buttonHeight }, "START GAME");
    buttons[2] = new Button({ (float)buttonX, (float)mapEditorButtonY, (float)buttonWidth, (float)buttonHeight }, "MAP EDITOR");
}
MainMenu::~MainMenu()
{
    UnloadTexture(background);
    for (int i = 0; i < 3; i++)
        delete buttons[i];
}

void MainMenu::update(float deltaTime) 
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (buttons[0]->press())
            Engine::getEngine()->setScene(new GameLearnScene());
        else if (buttons[1]->press()) {
            SetExitKey(0);
            Engine::getEngine()->setScene(new Game(false));
        }
        else if (buttons[2]->press())
            Engine::getEngine()->setScene(new MapEditor());
    }
}

void MainMenu::draw() 
{
    DrawTexturePro(background, { 0,0,(float)background.width,(float)background.height }, { 0,0,(float)GetScreenWidth(),(float)GetScreenHeight() }, { 0,0 }, 0, WHITE);
    //DrawTexture(background, 0, 0, WHITE);
    for (int i = 0; i < 3; i++)
        buttons[i]->draw();
}
