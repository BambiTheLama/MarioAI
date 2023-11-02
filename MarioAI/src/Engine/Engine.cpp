#include "Engine.h"
#include "raylib.h"
#include "ObjectToClone.h"
#include "../GameObject/GameObject.h"
#include "Game.h"
#include "MapEditor.h"
#include "MainMenu.h"
#include <time.h>
Engine* Engine::e = NULL;


Color backgroundColor;

Engine::Engine()
{
	srand(time(NULL));
	InitWindow(1600, 900, "Super Czesiek AI");
	CustomFont::loadFont();
	//SetTargetFPS(60);
	createObjectToCopy();
	s = new MainMenu();
	e = this;
}

Engine::~Engine()
{
	delete s;
	for (auto t : GameObject::texturesLoaded)
		UnloadTexture(t.texture);
	GameObject::texturesLoaded.clear();

	deleteObjectToCopy();
	CustomFont::unloadFont();
	CloseWindow();
}

void Engine::start()
{
	float deltaTime = 0;
	double time = 0;
	double time2 = GetTime();
	backgroundColor = GetColor(0x688bffff);;
	while (!WindowShouldClose())
	{
		time = time2;
		time2 = GetTime();
		deltaTime = time2 - time;
		if(s)
			s->update(deltaTime);
		BeginDrawing();
		ClearBackground(backgroundColor);
		if(s)
			s->draw();

		int fps = GetFPS();
		DrawText(TextFormat("FPS: %02d", fps), 1400, 0, 20, GREEN);
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