
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
private:
	bool isRunning = true;
	class Graphics* graphics;
};