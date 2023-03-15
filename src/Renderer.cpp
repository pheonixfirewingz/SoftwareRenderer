#include "Renderer.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _OPENMP
#    include <omp.h>
#endif
#include <inttypes.h>
#include <render/Ray.hpp>
#include <util/Math.hpp>
#include <util/Util.hpp>

Renderer::~Renderer()
{
    delete screen_data;
}

Renderer::Renderer(int width, int height)
    : internal_width(width)
    , internal_height(height)
{
    screen_data = new Pixel[width * height];
}

void Renderer::clearScreen(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
    Pixel pixel = Pixel({r, g, b, a});
    __m128i pixel_data = _mm_set_epi32(r, g, b, a);
    uint32_t pixel_count = internal_width * internal_height;
    uint32_t i = 0;
    for (; i + 3 < pixel_count; i += 4)
        _mm_storeu_si128((__m128i *)&screen_data[i], pixel_data);
    for (; i < pixel_count; i++)
        screen_data[i] = pixel;
}

void Renderer::resizeScreen(int width, int height)
{
    screen_data =
        static_cast<Pixel *>(realloc(screen_data, static_cast<size_t>(internal_width = width) *
                                                      static_cast<size_t>(internal_height = height) * sizeof(Pixel)));
}

void Renderer::setViewportPosition(const uint64_t, glm::vec3 position)
{
    viewport_position = position;
}

void Renderer::setViewPortRotation(const uint64_t, glm::vec4 rotation)
{
    viewport_rotation = rotation;
}

uint64_t Renderer::genLight(glm::vec4 colour)
{
    for (uint64_t i = 0; i < (REFRACTAL_MAX_LIGHTS - 1); i++)
    {
        if (lights[i].is_free)
        {
            lights[i].is_free = false;
            lights[i].light.setColour(colour);
            return i;
        }
    }
    return REFRACTAL_MAX_LIGHTS;
}

void Renderer::setLightPosition(const uint64_t id, glm::vec3 position)
{
    lights[id].light.setPosition(position);
}

void Renderer::setLightRotation(const uint64_t id, glm::vec4 rotation)
{
    lights[id].light.setRotation(rotation);
}

void Renderer::destroyLight(const uint64_t id)
{
    lights[id].is_free = true;
    lights[id].light.reset();
}

uint64_t Renderer::genMesh()
{
    for (uint64_t i = 0; i < (REFRACTAL_MAX_MESH - 1); i++)
    {
        if (buffers[i].is_free)
        {
            buffers[i].is_free = false;
            return i;
        }
    }
    return REFRACTAL_MAX_MESH;
}

void Renderer::setMeshPosition(const uint64_t id, glm::vec3 new_position)
{
    buffers[id].position = new_position;
}

void Renderer::setMeshRotation(const uint64_t id, glm::vec3 new_rotation)
{
    buffers[id].rotation = new_rotation;
}

void Renderer::transferData(const uint64_t id, const uint8_t type, const size_t buffer_size, void *data)
{
    switch (type)
    {
    case REFRACTAL_VERTEX_BUFFER: {
        buffers[id].vertices.reserve(buffer_size);
        for (size_t buffer_index = 0; buffer_index < buffer_size; buffer_index++)
            buffers[id].vertices.emplace_back(*((Vertex *)data + buffer_index));
    }
    break;
    case REFRACTAL_INDEX_BUFFER: {
        std::vector<Vertex> old;
        old.reserve(buffers[id].vertices.size() * 3);
        for (Triangle &tri : buffers[id].vertices)
        {
            old.emplace_back(tri.point_0);
            old.emplace_back(tri.point_1);
            old.emplace_back(tri.point_2);
        }
        std::vector<Vertex> new_;
        size_t vs = buffers[id].vertices.size();
        new_.reserve(vs * 3);
        std::vector<uint32_t> indexes;
        indexes.reserve(buffer_size);
        for (size_t buffer_index = 0; buffer_index < buffer_size; buffer_index++)
            indexes.emplace_back(*((uint32_t *)data + buffer_index));
        buffers[id].vertices.clear();
        for (size_t buffer_index = 0; buffer_index < buffer_size; buffer_index++)
            new_.push_back(old[indexes[buffer_index]]);
        buffers[id].vertices.clear();
        buffers[id].vertices.reserve(vs);

        for (size_t buffer_index = 2; buffer_index < (vs * 3); buffer_index += 3)
            buffers[id].vertices.emplace_back(
                Triangle(new_[buffer_index - 2], new_[buffer_index - 1], new_[buffer_index]));
    }
    break;
    default:
        break;
    }
}

void Renderer::destroyMesh(const uint64_t id)
{
    buffers[id].is_free = true;
    buffers[id].vertices.clear();
}

void Renderer::processPixel(const uint64_t , const uint64_t )
{
}

void Renderer::render()
{
    clearScreen(255, 0, 55, 55);

    for (size_t i = 0; i < REFRACTAL_MAX_MESH; i++)
    {
        if (buffers[i].is_free)
            continue;
        buffers[i].vertices_transformed.resize(buffers[i].vertices.size());

        // Find the maximum coordinate value among all the vertices in the mesh
        float max_coord = 0.0f;
        for (int j = 0; j < buffers[i].vertices.size(); j++)
        {
            glm::vec3 vertex = buffers[i].vertices[j].point_0.position;
            max_coord = std::max(max_coord, std::max(std::max(vertex.x, vertex.y), vertex.z));
            vertex = buffers[i].vertices[j].point_1.position;
            max_coord = std::max(max_coord, std::max(std::max(vertex.x, vertex.y), vertex.z));
            vertex = buffers[i].vertices[j].point_2.position;
            max_coord = std::max(max_coord, std::max(std::max(vertex.x, vertex.y), vertex.z));
        }

        // Scale the vertices by the maximum coordinate value
        float scale_factor = 1.0f / max_coord;
        for (int j = 0; j < buffers[i].vertices.size(); j++)
        {
            buffers[i].vertices_transformed[j].point_0.position =
                buffers[i].position + buffers[i].rotation * buffers[i].vertices[j].point_0.position * scale_factor;
            buffers[i].vertices_transformed[j].point_1.position =
                buffers[i].position + buffers[i].rotation * buffers[i].vertices[j].point_1.position * scale_factor;
            buffers[i].vertices_transformed[j].point_2.position =
                buffers[i].position + buffers[i].rotation * buffers[i].vertices[j].point_2.position * scale_factor;
        }
    }
#ifdef _OPENMP
#    pragma omp parallel for
#endif
    for (int64_t y = 0; y < internal_height; y++)
        for (int64_t x = 0; x < internal_width; x++)
            processPixel(x, y);
}

const void *Renderer::getScreenData() const noexcept
{
    return (const void *)screen_data;
}
