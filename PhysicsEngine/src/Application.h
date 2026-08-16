#include "Graphics.h"
class Application
{
public:
	Application();
	~Application();

	void Setup();
	void Render();
	void Input();
	void Update();
	void Clean();
	const bool GetIsRunning() { return isRunning; };
	float currentFrameTime;
	float previousFrameTime;
	float deltaTime;
	glm::vec2 windowCenter;
private:
	bool isRunning = true;
	class Graphics* graphics;
	int currentMouseX, currentMouseY;
	int prevMouseX, prevMouseY;

};