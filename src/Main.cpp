#include "App.h"
#include "Renderer.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
class TestApp : public App
{
	Renderer *renderer;
public:
	TestApp(int width, int height,const char* title) : App(width,height,title){
		run();
	}
protected:
	void init(int width, int height) final override
	{
		renderer = new Renderer(width,height);
	}

	void resizeCallback(int width, int height) final override
	{
		renderer->resizeScreen(width,height);
	}

	

	const unsigned int* render(int, int) final override
	{
		renderer->clearScreen(255,0,55,55);
		
		/*for (int y = 0; y <= height; y++) {
			for (int x = 0; x <= width; x++)
			{
				screen_data[y + x] = formatToRGBA(255,rand() % 255, rand() % 255, rand() % 255);
			}
		}*/
		return renderer->getScreenData();
	}

	void cleanup() final override
	{
		delete renderer;	
	}
};

int main()
{
	TestApp app(500,500,"software Render App");
	return 0;
}