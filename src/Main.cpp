#include "App.h"
#include <Renderer.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define FAST_OBJ_IMPLEMENTATION
#include <fast_obj.h>
#include <future>
#include <imgui.h>

std::vector<glm::vec3> parseObj(const char *path)
{
    fastObjMesh *obj = fast_obj_read(path);
    if (!obj)
    {
        printf("Error loading %s: file not found\n", path);
        return std::vector<glm::vec3>();
    }

    size_t total_indices = 0;

    for (unsigned int i = 0; i < obj->face_count; ++i)
        total_indices += 3 * (obj->face_vertices[i] - 2);

    std::vector<glm::vec3> vertices(total_indices);

    size_t vertex_offset = 0;
    size_t index_offset = 0;

    for (unsigned int i = 0; i < obj->face_count; ++i)
    {
        for (unsigned int j = 0; j < obj->face_vertices[i]; ++j)
        {
            fastObjIndex gi = obj->indices[index_offset + j];

            glm::vec3 v = {obj->positions[gi.p * 3 + 0], obj->positions[gi.p * 3 + 1], obj->positions[gi.p * 3 + 2]};

            // triangulate polygon on the fly; offset-3 is always the first polygon vertex
            if (j >= 3)
            {
                vertices[vertex_offset + 0] = vertices[vertex_offset - 3];
                vertices[vertex_offset + 1] = vertices[vertex_offset - 1];
                vertex_offset += 2;
            }

            vertices[vertex_offset] = v;
            vertex_offset++;
        }

        index_offset += obj->face_vertices[i];
    }

    fast_obj_destroy(obj);
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
        /*const glm::vec3 v_buffer[] = {
            {-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f},  {-1.0f, 1.0f, 1.0f},   // triangle 1
            {1.0f, 1.0f, -1.0f},   {-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f},  // triangle 2
            {1.0f, -1.0f, 1.0f},   {-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f},  // triangle 3
            {1.0f, 1.0f, -1.0f},   {1.0f, -1.0f, -1.0f},  {-1.0f, -1.0f, -1.0f}, // triangle 4
            {-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, 1.0f},   {-1.0f, 1.0f, -1.0f},  // triangle 5
            {1.0f, -1.0f, 1.0f},   {-1.0f, -1.0f, 1.0f},  {-1.0f, -1.0f, -1.0f}, // triangle 6
            {-1.0f, 1.0f, 1.0f},   {-1.0f, -1.0f, 1.0f},  {1.0f, -1.0f, 1.0f},   // triangle 7
            {1.0f, 1.0f, 1.0f},    {1.0f, -1.0f, -1.0f},  {1.0f, 1.0f, -1.0f},   // triangle 8
            {1.0f, -1.0f, -1.0f},  {1.0f, 1.0f, 1.0f},    {1.0f, -1.0f, 1.0f},   // triangle 9
            {1.0f, 1.0f, 1.0f},    {1.0f, 1.0f, -1.0f},   {-1.0f, 1.0f, -1.0f},  // triangle 10
            {1.0f, 1.0f, 1.0f},    {-1.0f, 1.0f, -1.0f},  {-1.0f, 1.0f, 1.0f},   // triangle 11
            {1.0f, 1.0f, 1.0f},    {-1.0f, 1.0f, 1.0f},   {1.0f, -1.0f, 1.0f},   // triangle 12
        };*/
        std::vector<glm::vec3> mesh = parseObj(ROOT_PATH "/dependances/cornell-box.obj");
        renderer->transferData(mesh_id, REFRACTAL_VERTEX_BUFFER, mesh.size() / 3, (void *)mesh.data());
        renderer->setMeshPosition(mesh_id, {0, -1, 0});
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