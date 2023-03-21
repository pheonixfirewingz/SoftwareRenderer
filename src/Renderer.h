#pragma once
#include <atomic>
#include <cstdint>
#include <render/Camera.hpp>
#include <render/Ray.hpp>
#include <util/Sse3.h>
#include <vector>

struct RefractalTriangle
{
    glm::vec3 point_0{0.f, 0.f, 0.f};
    glm::vec2 point_0_uv{0.f, 0.f};
    glm::vec3 point_1{0.f, 0.f, 0.f};
    glm::vec2 point_1_uv{0.f, 0.f};
    glm::vec3 point_2{0.f, 0.f, 0.f};
    glm::vec2 point_2_uv{0.f, 0.f};
    RefractalTriangle() = default;
    RefractalTriangle(glm::vec3 point_0_in, glm::vec3 point_1_in, glm::vec3 point_2_in, bool normalize = true)
        : point_0(normalize ? glm::normalize(point_0_in) : point_0_in)
        , point_1(normalize ? glm::normalize(point_1_in) : point_1_in)
        , point_2(normalize ? glm::normalize(point_2_in) : point_2_in)
    {
    }

    RefractalTriangle(glm::vec3 point_0_in, glm::vec3 point_1_in, glm::vec3 point_2_in, glm::vec2 point_0_uv_in,
                      glm::vec2 point_1_uv_in, glm::vec2 point_2_uv_in, bool normalize = true)
        : point_0(normalize ? glm::normalize(point_0_in) : point_0_in)
        , point_1(normalize ? glm::normalize(point_1_in) : point_1_in)
        , point_2(normalize ? glm::normalize(point_2_in) : point_2_in)
        , point_0_uv(point_0_uv_in)
        , point_1_uv(point_1_uv_in)
        , point_2_uv(point_2_uv_in)
    {
    }
};

#define REFRACTAL_MAX_LIGHTS 0x01
#define REFRACTAL_MAX_MESH 0x01
#define REFRACTAL_VERTEX_BUFFER 0x00
#define REFRACTAL_VERTEX_FORMAT_XYZ 0x00
#define REFRACTAL_VERTEX_FORMAT_XYZ_UV 0x01
// TODO: add error system
class Renderer
{
    struct Pixel
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    } *screen_data;
    uint32_t internal_width;
    uint32_t internal_height;
    glm::mat4 proj;
    glm::vec3 viewport_position{0.f, 0.f, 0.f};
    glm::vec3 viewport_rotation{0.f, 1.f, 0.f};
    glm::uvec3 base_colour{0, 55, 55};

    struct LightRegister
    {
        float luminace = 1.0f;
        glm::vec3 colour{1.0f, 1.0f, 1.0f};
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::vec3 rotation{0.0f, 0.0f, 0.0f};
        bool is_free = true;

        void set()
        {
            is_free = false;
        }

        void free()
        {
            luminace = 1.0f;
            colour = {1.0f, 1.0f, 1.0f};
            position = {0.0f, 0.0f, 0.0f};
            rotation = {0.0f, 0.0f, 0.0f};
            is_free = true;
        }

    } lights[REFRACTAL_MAX_LIGHTS];

    struct MeshRegister
    {
        std::vector<RefractalTriangle> original_vertices;
        std::vector<RefractalTriangle> vertices;
        glm::vec3 position{0, 0, 0};
        glm::vec3 rotation{0, 0, 0};
        bool hasHit(const Ray &ray, RayHitInfomation &info) noexcept;
        bool is_free = true;

        void set()
        {
            is_free = false;
        }

        void free()
        {
            is_free = true;
            original_vertices.clear();
            vertices.clear();
            position = {0, 0, 0};
            rotation = {0, 0, 0};
        }
    } buffers[REFRACTAL_MAX_MESH];

  protected:
    size_t getVertexFormatSize(const uint8_t format) const noexcept;

  public:
    Renderer(int width, int height);
    ~Renderer()
    {
        delete screen_data;
    }

    // view port
    void setViewportPosition(const uint64_t, const glm::vec3 position) noexcept
    {
        viewport_position = position;
    }

    void setViewportRotation(const uint64_t, const glm::vec3 rotation) noexcept
    {
        viewport_rotation = rotation;
    }

    // lights
    uint64_t genLight();
    void setLightLuminance(const uint64_t id, const float luminance) noexcept
    {
        lights[id].luminace = luminance;
    }

    void setLightColour(const uint64_t id, const glm::vec3 colour) noexcept
    {
        lights[id].colour = colour;
    }

    void setLightPosition(const uint64_t id, glm::vec3 position) noexcept
    {
        lights[id].position = position;
    }

    void setLightRotation(const uint64_t id, glm::vec4 rotation) noexcept
    {
        lights[id].rotation = rotation;
    }

    void destroyLight(const uint64_t id) noexcept
    {
        lights[id].free();
    }

    // meshes
    uint64_t genMesh();
    void setMeshPosition(const uint64_t id, const glm::vec3 new_position) noexcept
    {
        buffers[id].position = new_position;
    }

    void setMeshRotation(const uint64_t id, const glm::vec3 new_rotation) noexcept
    {
        buffers[id].rotation = new_rotation;
    }

    void transferData(const uint64_t id, const uint8_t type, const uint8_t format, const size_t buffer_size,
                      const void *data);

    void destroyMesh(const uint64_t id) noexcept
    {
        buffers[id].free();
    }

    //misc
    void setScreenClearColour(const glm::uvec3 colour)
    {
        base_colour = colour;
    }
    void resizeScreen(int width, int height);
    void render();

    const void *getScreenData() const noexcept
    {
        return (const void *)screen_data;
    }
};