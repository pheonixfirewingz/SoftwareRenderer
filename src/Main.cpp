#include "App.h"
#include <Renderer.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <future>
#include <imgui.h>

class TestApp : public App
{
    Renderer *renderer;
    Camera cam;
    uint64_t light_id;
    uint64_t mesh_id;

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
        light_id = renderer->genLight({0, 0, 0, 0});
        mesh_id = renderer->genMesh();
        const glm::vec3 v_buffer[] = {
            {-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f},  {-1.0f, 1.0f, 1.0f},   // triangle
            {1.0f, 1.0f, -1.0f},   {-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f},  // triangle
            {1.0f, -1.0f, 1.0f},   {-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f},  // triangle
            {1.0f, 1.0f, -1.0f},   {1.0f, -1.0f, -1.0f},  {-1.0f, -1.0f, -1.0f}, // triangle
            {-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, 1.0f},   {-1.0f, 1.0f, -1.0f},  // triangle
            {1.0f, -1.0f, 1.0f},   {-1.0f, -1.0f, 1.0f},  {-1.0f, -1.0f, -1.0f}, // triangle
            {-1.0f, 1.0f, 1.0f},   {-1.0f, -1.0f, 1.0f},  {1.0f, -1.0f, 1.0f},   // triangle
            {1.0f, 1.0f, 1.0f},    {1.0f, -1.0f, -1.0f},  {1.0f, 1.0f, -1.0f},   // triangle
            {1.0f, -1.0f, -1.0f},  {1.0f, 1.0f, 1.0f},    {1.0f, -1.0f, 1.0f},   // triangle
            {1.0f, 1.0f, 1.0f},    {1.0f, 1.0f, -1.0f},   {-1.0f, 1.0f, -1.0f},  // triangle
            {1.0f, 1.0f, 1.0f},    {-1.0f, 1.0f, -1.0f},  {-1.0f, 1.0f, 1.0f},   // triangle
            {1.0f, 1.0f, 1.0f},    {-1.0f, 1.0f, 1.0f},   {1.0f, -1.0f, 1.0f},   // triangle
        };
        renderer->transferData(mesh_id, REFRACTAL_VERTEX_BUFFER, 3 * 12, (void *)&v_buffer[0]);
        // Mesh mesh = parseObj(ROOT_PATH "/dependances/Sponza/sponza.obj");
        // renderer->transferData(mesh_id, REFRACTAL_VERTEX_BUFFER, mesh.vertices.size(), mesh.vertices.data());
        // renderer->transferData(mesh_id, REFRACTAL_INDEX_BUFFER, mesh.indices.size(), mesh.indices.data());
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

        // static std::thread t([this]() {
        renderer->setViewportPosition(0, cam.getPos());
        renderer->render();
        //});
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