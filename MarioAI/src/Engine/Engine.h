#pragma once
#include "Scene.h"
#include "raylib.h"

class Engine
{
	static Engine* e;
	Scene* s;
	Image icon;
	Color backgroundColor;
public:
	Engine();

	~Engine();

	void start();

	void setScene(Scene* s);

	static Engine* getEngine() { return e; }
};

