#include "App.h"
#include <Renderer.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <imgui.h>
#include <meshoptimizer.h>
#include <vector>
#define FAST_OBJ_IMPLEMENTATION
#include <fast_obj.h>
// copied from - meshoptimizer

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

Mesh parseObj(const char *path)
{
    fastObjMesh *obj = fast_obj_read(path);
    if (!obj)
    {
        printf("Error loading %s: file not found\n", path);
        return Mesh();
    }

    size_t total_indices = 0;

    for (unsigned int i = 0; i < obj->face_count; ++i)
        total_indices += 3 * (obj->face_vertices[i] - 2);

    std::vector<Vertex> vertices(total_indices);

    size_t vertex_offset = 0;
    size_t index_offset = 0;

    for (unsigned int i = 0; i < obj->face_count; ++i)
    {
        for (unsigned int j = 0; j < obj->face_vertices[i]; ++j)
        {
            fastObjIndex gi = obj->indices[index_offset + j];

            Vertex v = {
                {obj->positions[gi.p * 3 + 0], obj->positions[gi.p * 3 + 1], obj->positions[gi.p * 3 + 2]},
                {obj->normals[gi.n * 3 + 0], obj->normals[gi.n * 3 + 1], obj->normals[gi.n * 3 + 2]},
                {obj->texcoords[gi.t * 2 + 0], obj->texcoords[gi.t * 2 + 1]}};

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

    Mesh result;

    std::vector<unsigned int> remap(total_indices);

    size_t total_vertices =
        meshopt_generateVertexRemap(&remap[0], NULL, total_indices, &vertices[0], total_indices, sizeof(Vertex));

    result.indices.resize(total_indices);
    meshopt_remapIndexBuffer(&result.indices[0], NULL, total_indices, &remap[0]);

    result.vertices.resize(total_vertices);
    meshopt_remapVertexBuffer(&result.vertices[0], &vertices[0], total_indices, sizeof(Vertex), &remap[0]);

    return result;
}

// end copy

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
    void init(int width, int height) final override
    {
        cam.updateView(width,height);
        light_id = renderer->genLight({0,0,0,0});
        mesh_id = renderer->genMesh();
        Mesh mesh = parseObj(ROOT_PATH"/dependances/Sponza/sponza.obj");
        renderer->transferData(mesh_id,REFRACTAL_VERTEX_BUFFER,mesh.vertices.size(),mesh.vertices.data());
        renderer->transferData(mesh_id,REFRACTAL_INDEX_BUFFER,mesh.indices.size(),mesh.indices.data());
        renderer->setMeshPosition(mesh_id,{0,0,0});
        renderer->setMeshRotation(mesh_id,{0,1,0});
    }

    void resizeCallback(int width, int height) final override
    {
        cam.updateView(width,height);
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
        renderer->setProjectionMatrix(cam.getCameraMatrix());
        renderer->clearScreen(255, 0, 55, 55);
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
    TestApp app(1280, 720, "software Render App");
    return 0;
}