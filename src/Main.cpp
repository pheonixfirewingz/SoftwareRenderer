#include "App.h"
#include <Renderer.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef IS_GCC
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wunused-value"
#    pragma GCC diagnostic ignored "-Wsign-compare"
#    pragma GCC diagnostic ignored "-Wdouble-promotion"
#endif
#include <OBJ_Loader.h>
#ifdef IS_GCC
#    pragma GCC diagnostic pop
#endif
#include <future>
#include <imgui.h>

static std::vector<glm::vec3> parseObj(const char *path)
{
    std::vector<glm::vec3> vertices;
    objl::Loader Loader;
    if (!Loader.LoadFile(path))
        throw std::runtime_error(std::string("couldn't load file ") + path);
    for (size_t i = 0; i < Loader.LoadedMeshes.size(); i++)
    {
        objl::Mesh curMesh = Loader.LoadedMeshes[i];
        for (size_t j = 0; j < curMesh.Vertices.size(); j++)
            vertices.push_back(glm::vec3(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y,
                                         curMesh.Vertices[j].Position.Z));
    }
    return vertices;
}

class TestApp : public App
{
    Renderer *renderer;
    Camera cam;
    uint64_t light_id = 0;
    uint64_t mesh_id = 0;

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
        light_id = renderer->genLight({1, 1, 1, 1});
        mesh_id = renderer->genMesh();
        std::vector<glm::vec3> mesh = parseObj(ROOT_PATH "/dependances/sponza.obj");
        renderer->transferData(mesh_id, REFRACTAL_VERTEX_BUFFER, mesh.size() * 3, (void *)mesh.data());
        renderer->setMeshPosition(mesh_id, {0, 0, 0});
        renderer->setMeshRotation(mesh_id, {0, 0, 0});
        cam.update(this);
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
        renderer->setViewportRotation(0, cam.getRot());
        renderer->setViewportPosition(0, cam.getPos());
        static std::thread k([this]() { renderer->render(); });
        return renderer->getScreenData();
    }

    void cleanup() final override
    {
        renderer->destroyMesh(mesh_id);
        renderer->destroyLight(light_id);
        delete renderer;
    }
};

int main()
{
    TestApp app(500, 500, "software Render App");
    return 0;
}