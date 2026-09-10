#include "Application.h"
#include "SDL.h"

Application::Application()
{
}

Application::~Application()
{
}

void Application::Setup()
{
	graphics = new Graphics();
	SDL_SetRelativeMouseMode(SDL_TRUE);
	windowCenter.x = WINDOW_WIDTH / 2;
	windowCenter.y = WINDOW_HEIGHT / 2;
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

			//	if (sdlEvent.key.keysym.sym == SDLK_a)
			//		graphics->CameraMoveLeft();

			//	if (sdlEvent.key.keysym.sym == SDLK_d)
			//		graphics->CameraMoveRight();

			//	if (sdlEvent.key.keysym.sym == SDLK_w)
			//		graphics->CameraMoveForward();

			//	if (sdlEvent.key.keysym.sym == SDLK_s)
			//		graphics->CameraMoveBackward();

			//	if (sdlEvent.key.keysym.sym == SDLK_q)
			//		graphics->CameraMoveDown();

			//	if (sdlEvent.key.keysym.sym == SDLK_e)
			//		graphics->CameraMoveUp();

			//	if (sdlEvent.key.keysym.sym == SDLK_LEFT)
			//		graphics->CameraMoveLeft();

			//	if (sdlEvent.key.keysym.sym == SDLK_RIGHT)
			//		graphics->CameraMoveLeft();

			//	break;
			case SDL_MOUSEMOTION:
				graphics->CameraAddRotation(glm::vec2(sdlEvent.motion.xrel, sdlEvent.motion.yrel));
				break;
		}

	graphics->ProcessInput(SDL_GetKeyboardState(nullptr), deltaTime);

}

void Application::Update()
{
	previousFrameTime = currentFrameTime;
	currentFrameTime = SDL_GetTicks() / 1000.f;
	deltaTime = currentFrameTime - previousFrameTime;
	graphics->Update(deltaTime);
}

void Application::Clean()
{
	delete graphics;
}
