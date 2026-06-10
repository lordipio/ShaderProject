#include "Application.h"
#include "SDL.h"
#include "Graphics.h"

Application::Application()
{
}

Application::~Application()
{
}

void Application::Setup()
{
	graphics = new Graphics();
}

void Application::Render()
{
	graphics->Render();
}

void Application::Input()
{
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent))
		switch (sdlEvent.type)
		{
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
				isRunning = false;
			break;
		}
}

void Application::Update()
{
	previousFrameTime = currentFrameTime;
	currentFrameTime = SDL_GetTicks() / 1000.f;
	graphics->Update(currentFrameTime - previousFrameTime);
}

void Application::Clean()
{
	delete graphics;
}
