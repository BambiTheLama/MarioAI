#pragma once
#include "Scene.h"
#include "raylib.h"
#include <string>
class Button {
    Rectangle pos;
public:
    std::string text;

    Button(Rectangle pos, std::string text);

    void draw();

    bool press();
};

class MainMenu :
    public Scene
{
#define NButtons 4
#define NMaps 3
    Button *buttons[NButtons];
    Texture2D background;
    int mapID = 1;
public:

    MainMenu();

    ~MainMenu();

    void update(float deltaTIme);

    void draw();
};

