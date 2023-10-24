#pragma once
#include "Scene.h"
class MainMenu :
    public Scene
{
public:
    MainMenu();

    void update(float deltaTIme);

    void draw();
};

