#pragma once
#include "Scene.h"
#include "Chunk.h"
#include "ObjectToClone.h"

class MapEditor : public Scene {
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
  int mapID;

public:
  MapEditor(int mapID);

  ~MapEditor();

  void update(float deltaTime);

  void pressMouse();

  void placeBlock();

  void removeObject();

  void newChunk();

  void deleteChunk();

  void saveMap();

  void draw();
};
