#pragma once
#include "Scene.h"
class MainMenu :
    public Scene
{
public:

    MainMenu();

    ~MainMenu();

    void update(float deltaTIme);

    void draw();
};

