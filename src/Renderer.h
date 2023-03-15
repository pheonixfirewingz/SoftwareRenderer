#pragma once
#include <cstdint>
#include <render/Camera.hpp>
#include <render/Light.hpp>
#include <render/Vertex.hpp>
#include <util/Sse3.h>
#include <vector>
#include <atomic>

#define REFRACTAL_MAX_LIGHTS 100
#define REFRACTAL_MAX_MESH 100
#define REFRACTAL_VERTEX_BUFFER 1
#define REFRACTAL_INDEX_BUFFER 0
class Renderer
{
    uint32_t internal_width;
    uint32_t internal_height;
    glm::mat4x4 proj_matrix;
    struct Pixel
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    } * screen_data;
    struct Lights
    {
        Light light;
        bool is_free = true;
    } lights[REFRACTAL_MAX_LIGHTS];

    struct Mesh
    {
        std::vector<Vertex> vertices;
        std::vector<Vertex> vertices_transformed;
        std::vector<uint32_t> indices;
        glm::vec3 position;
        glm::vec3 rotation;
        bool is_free = true;
    } buffers[REFRACTAL_MAX_MESH];

  protected:
    void processPixel(const uint64_t x, const uint64_t y);

  public:
    Renderer(int width, int height);
    ~Renderer();
    void clearScreen(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
    void resizeScreen(int width, int height);

    void setProjectionMatrix(glm::mat4x4 mat);


    // lights
    uint64_t genLight(glm::vec4 colour);
    void setLightPosition(const uint64_t id, glm::vec3 position);
    void setLightRotation(const uint64_t id, glm::vec4 rotation);
    void destroyLight(const uint64_t id);

    // meshes
    uint64_t genMesh();
    void setMeshPosition(const uint64_t id, glm::vec3 position);
    void setMeshRotation(const uint64_t id, glm::vec3 rotation);
    void transferData(const uint64_t id, const uint8_t type, const size_t buffer_size, void *data);
    void destroyMesh(const uint64_t id);

    void render();

    const void *getScreenData() const noexcept;
};