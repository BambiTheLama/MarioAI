#pragma once
#include "Scene.h"
#include "Chunk.h"
#include "ObjectToClone.h"

class MapEdytor :
    public Scene
{
    Camera2D camera;
    std::vector<Chunk*> chunks;
    std::vector<GameObject*> blocks;
    std::vector<GameObject*> otherObjects;
    GameObject* usingObj;
    bool displayBlock = false;
    bool menuLeft = false;
    bool usingBlock = false;
    int ObjectId = 0;
    Rectangle menuPos;
public:
    MapEdytor();

    ~MapEdytor();

    void update(float delataTime);

    void pressMouse();

    void newChunk();

    void saveMap();

    void draw();
};

