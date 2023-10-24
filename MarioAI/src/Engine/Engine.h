#pragma once
#include "Scene.h"

class Engine
{
	static Engine* e;
	Scene* s;
public:
	Engine();

	~Engine();

	void start();

	void setScene(Scene* s);

	static Engine* getEngine() { return e; }
};

