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

struct vertex
{
    glm::vec3 pos;
    float u, v;
    vertex(glm::vec3 pos_in, float u_in, float v_in)
        : pos(pos_in)
        , u(u_in)
        , v(v_in)
    {
    }
};

static std::vector<vertex> parseObj(const char *path)
{
    std::vector<vertex> vertices;
    objl::Loader Loader;
    if (!Loader.LoadFile(path))
        throw std::runtime_error(std::string("couldn't load file ") + path);
    for (size_t i = 0; i < Loader.LoadedMeshes.size(); i++)
    {
        objl::Mesh curMesh = Loader.LoadedMeshes[i];
        for (size_t j = 0; j < curMesh.Vertices.size(); j++)
            vertices.push_back(vertex(glm::vec3(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y,
                                                curMesh.Vertices[j].Position.Z),
                                      curMesh.Vertices[j].TextureCoordinate.X,
                                      curMesh.Vertices[j].TextureCoordinate.Y));
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
        light_id = renderer->genLight();
        mesh_id = renderer->genMesh();
        std::vector<vertex> mesh = parseObj(ROOT_PATH "/dependances/box.obj");
        renderer->transferData(mesh_id, REFRACTAL_VERTEX_BUFFER, REFRACTAL_VERTEX_FORMAT_XYZ_UV,
                               mesh.size() * sizeof(vertex), (void *)mesh.data());
        renderer->setMeshPosition(mesh_id, {0, 0,6});
        renderer->setMeshRotation(mesh_id, {0, 180,0});
        renderer->setLightPosition(light_id, {0, 5,6});
        renderer->setLightRotation(light_id, {0, 0, -1.0f});
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
        renderer->setViewportPosition(0, cam.getPos());
        renderer->render();
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