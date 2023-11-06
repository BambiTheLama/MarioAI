#include "MainMenu.h"
#include "raylib.h"
#include "Engine.h"
#include "Game.h"
#include "MapEditor.h"
#include "GameLearnScene.h"
#include <fstream>

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
    std::ifstream reader;
    reader.open("menu");
    if (reader.is_open())
    {
        reader >> mapID;
    }
    reader.close();
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
    const int choseMapButtonY = screenHeight / 3 + 2 * (buttonHeight + buttonSpacing);
    
    buttons[0] = new Button({ (float)buttonX, (float)aiGameButtonY, (float)buttonWidth, (float)buttonHeight }, "AI GAME");
    buttons[1] = new Button({ (float)buttonX, (float)startGameButtonY, (float)buttonWidth, (float)buttonHeight }, "START GAME");
    buttons[2] = new Button({ (float)buttonX, (float)mapEditorButtonY, (float)buttonWidth, (float)buttonHeight }, "MAP EDITOR");
    buttons[3] = new Button({ (float)buttonX, (float)choseMapButtonY, (float)buttonWidth, (float)buttonHeight }, "MAP: "+ std::to_string(mapID));

}
MainMenu::~MainMenu()
{
    std::ofstream writer;
    writer.open("menu");
    if (writer.is_open())
    {
        writer << mapID;
    }
    writer.close();
    UnloadTexture(background);
    for (int i = 0; i < NButtons; i++)
        delete buttons[i];
}

void MainMenu::update(float deltaTime) 
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (buttons[0]->press())
            Engine::getEngine()->setScene(new GameLearnScene(mapID));
        else if (buttons[1]->press()) {
            SetExitKey(0);
            Engine::getEngine()->setScene(new Game(false,mapID));
        }
        else if (buttons[2]->press())
            Engine::getEngine()->setScene(new MapEditor(mapID));
        else if (buttons[3]->press())
        {
            mapID++;
            if (mapID > NMaps)
                mapID = 1;
            buttons[3]->text.pop_back();
            buttons[3]->text.push_back(std::to_string(mapID).at(0));
        }
    }
}

void MainMenu::draw() 
{
    DrawTexturePro(background, { 0,0,(float)background.width,(float)background.height }, { 0,0,(float)GetScreenWidth(),(float)GetScreenHeight() }, { 0,0 }, 0, WHITE);
    //DrawTexture(background, 0, 0, WHITE);
    for (int i = 0; i < NButtons; i++)
        buttons[i]->draw();
}
