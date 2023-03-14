#include "App.h"
#include <Renderer.h>
#include <imgui.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
class TestApp : public App
{
    Renderer *renderer;
    Camera<double> cam;

  public:
    TestApp(int width, int height, const char *title)
        : App(width, height, title)
    {
        renderer = new Renderer(width, height);
        run();
    }

  protected:
    void init(int, int) final override
    {
    }

    void resizeCallback(int width, int height) final override
    {
        renderer->resizeScreen(width, height);
    }

    void renderDebugUI() final override
    {
        ImGui::SetNextWindowSizeConstraints(ImVec2(float(110), float(55)), ImVec2(float(110), float(55)));
        ImGui::Begin("Debug");
        
        if (ImGui::Button("Toggle Vsync"))
        {
            static bool vsync = true;
            vsync = !vsync;
            glfwSwapInterval(vsync);
        }
        ImGui::End();
    }

    const void *render(int, int) final override
    {
        cam.update(this);
        renderer->clearScreen(255, 0, 55, 55);
        renderer->render();
        return renderer->getScreenData();
    }

    void cleanup() final override
    {
        delete renderer;
    }
};

int main()
{
    TestApp app(500, 500, "software Render App");
    return 0;
}