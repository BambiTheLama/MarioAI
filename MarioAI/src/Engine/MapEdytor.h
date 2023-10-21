#pragma once
#include "Scene.h"
class MapEdytor :
    public Scene
{

public:
    MapEdytor();

    void update(float delataTime);

    void draw();
};

