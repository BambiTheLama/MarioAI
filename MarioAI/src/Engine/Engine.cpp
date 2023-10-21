#include "Engine.h"
#include "raylib.h"
#include "ObjectToClone.h"
#include "../GameObject/GameObject.h"
#include "Game.h"

Engine::Engine()
{
	InitWindow(1600, 900, "MARIO");
	SetTargetFPS(60);
	createObjectToCopy();
	s = new Game();
}

Engine::~Engine()
{
	for (auto t : GameObject::texturesLoaded)
		UnloadTexture(t.texture);
	GameObject::texturesLoaded.clear();

	deleteObjectToCopy();
	CloseWindow();
}

void Engine::start()
{
	float deltaTime = 0;
	double time = 0;
	double time2 = GetTime();
	while (!WindowShouldClose())
	{
		time = time2;
		time2 = GetTime();
		deltaTime = time2 - time;
		if(s)
			s->update(deltaTime);
		BeginDrawing();
		ClearBackground(DARKBLUE);
		if(s)
			s->draw();

		DrawFPS(0, 0);
		EndDrawing();
	}
}

void Engine::setScene(Scene* s)
{ 
	if (this->s && s)
	{
		delete this->s;
		this->s = s;
	}
	else if (s)
	{
		this->s = s;
	}
}