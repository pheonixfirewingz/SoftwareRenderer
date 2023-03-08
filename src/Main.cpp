#include "App.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
class TestApp : public App
{
	uint32_t* screen_data;
public:
	TestApp(int width, int height,const char* title) : App(width,height,title) , screen_data(static_cast<uint32_t*>(malloc(width* height * sizeof(uint32_t)))){}
	~TestApp() override {
		free(screen_data);
		App::~App();
	}
protected:
	void resizeCallback(int width, int height) final override
	{
		screen_data = static_cast<uint32_t*>(realloc(screen_data, width * height * sizeof(uint32_t)));
	}

	inline int clampTo8Bit(int v) {
		v &= (v < 0) - 1;  // clamp < 0
		v |= -(v > 255); // clamp > 255
		return v & 0xFF; // mask out;
	}

	inline int ARGBclamped(int a, int r, int g, int b) {
		if (((a | r | g | b) & ~0xFF) != 0) {
			a = clampTo8Bit(a);
			r = clampTo8Bit(r);
			g = clampTo8Bit(g);
			b = clampTo8Bit(b);
		}
		return (r << 24) + (g << 16) + (b << 8) + (a << 0);
	}

	unsigned int* render(int width, int height) final override
	{
		memset(screen_data, 0, width * height * sizeof(uint32_t));
		for (int y = 0; y <= height; y++) {
			for (int x = 0; x <= width; x++)
			{
				screen_data[y + x] = ARGBclamped(255,0, 5, 50);
			}
		}
		return screen_data;
	}
};

int main()
{
	TestApp app(500,500,"software Render App");
	app.run();
	return 0;
}