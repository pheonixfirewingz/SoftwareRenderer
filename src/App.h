#pragma once
class App
{
	void* window;
protected:
	App(int width, int height,const char* title);
	virtual void resizeCallback(int width, int height) = 0;
	virtual unsigned int* render(int width, int height) = 0;
	virtual ~App();
public:
	void run();
};