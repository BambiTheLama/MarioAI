#pragma once
#include "Scene.h"
#include "raylib.h"
#include <string>
class Button {
    Rectangle pos;
    std::string text;
public:
    Button(Rectangle pos, std::string text);

    void draw();

    virtual bool press();
};

class MainMenu :
    public Scene
{
    Button *buttons[3];
    Texture2D background;
public:

    MainMenu();

    ~MainMenu();

    void update(float deltaTIme);

    void draw();
};

