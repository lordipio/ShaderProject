#include "Application.h"
#include <SDL.h>

int main(int argc, char* args[])
{
	Application app = Application();
	
	app.Setup();
	while (app.GetIsRunning())
	{
		app.Input();
		app.Update();
		app.Render();
	}
	app.Clean();
	return 0;
}