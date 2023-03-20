#pragma once
#include <atomic>
#include <cstdint>
#include <render/Camera.hpp>
#include <render/Light.hpp>
#include <render/Mesh.hpp>
#include <util/Sse3.h>
#include <vector>

#define REFRACTAL_MAX_LIGHTS 1
#define REFRACTAL_MAX_MESH 1
#define REFRACTAL_VERTEX_BUFFER 0
class Renderer
{
    uint32_t internal_width;
    uint32_t internal_height;
    glm::vec3 viewport_position{0.f, 0.f, 0.f};
    glm::vec3 viewport_rotation{0.f, 1.f, 0.f};
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

    struct MeshRegister
    {
        RefractalMesh mesh;
        bool is_free = true;
        
        void set()
        {
          is_free = false;
        }

        void free()
        {
          is_free = true;
          mesh = RefractalMesh();
        }
    } buffers[REFRACTAL_MAX_MESH];

  protected:
    void processPixel(const uint64_t x, const uint64_t y);
    bool markDirty = true;
  public:
    Renderer(int width, int height);
    ~Renderer();
    void resizeScreen(int width, int height);

    // view port
    void setViewportPosition(const uint64_t id, glm::vec3 position);
    void setViewportRotation(const uint64_t id, glm::vec3 rotation);

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