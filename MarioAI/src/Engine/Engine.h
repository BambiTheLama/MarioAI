#pragma once
#include "Scene.h"

class Engine
{
	Scene* s;
public:
	Engine();

	~Engine();

	void start();

	void setScene(Scene* s);
};

