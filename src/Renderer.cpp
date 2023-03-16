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
    for (uint64_t i = 0; i < (REFRACTAL_MAX_LIGHTS - 1); ++i)
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
    for (uint64_t i = 0; i < REFRACTAL_MAX_MESH; ++i)
    {
        if (buffers[i].is_free)
        {
            buffers[i].set();
            return i;
        }
    }
    return REFRACTAL_MAX_MESH;
}

void Renderer::setMeshPosition(const uint64_t id, glm::vec3 new_position)
{
    buffers[id].mesh.position = new_position;
    buffers[id].markDirty = true;
}

void Renderer::setMeshRotation(const uint64_t id, glm::vec3 new_rotation)
{
    buffers[id].mesh.rotation = new_rotation;
    buffers[id].markDirty = true;
}

void Renderer::transferData(const uint64_t id, const uint8_t type, const size_t buffer_size, void *data)
{
    switch (type)
    {
    case REFRACTAL_VERTEX_BUFFER: {
        buffers[id].mesh.vertices.reserve(buffer_size);
        for (size_t buffer_index = 0; buffer_index < buffer_size; buffer_index++)
            buffers[id].mesh.vertices.emplace_back(*((RefractalTriangle *)data + buffer_index));
    }
    break;
    default:
        break;
    }
    buffers[id].markDirty = true;
}

void Renderer::destroyMesh(const uint64_t id)
{
    buffers[id].free();
}

void Renderer::processPixel(const uint64_t x, const uint64_t y)
{
    glm::vec2 coord = glm::vec2((float)x / (float)internal_width, (float)y / (float)internal_height) * 2.0f - 1.0f;
    glm::vec3 colour(0, 0, 0);

    for (size_t i = 0; i < REFRACTAL_MAX_MESH; ++i)
    {
        MeshRegister &mesh = buffers[i];
        Ray ray(viewport_position, glm::vec3(coord.x, coord.y, -1.0f));
        if (mesh.is_free)
            continue;
        if (mesh.mesh.hasHit(ray))
            colour += mesh.mesh.colour;
    }
    screen_data[x + y * internal_width] =
        Pixel(uint8_t(std::clamp(colour.r, 0.f, 1.f) * 255.0f), uint8_t(std::clamp(colour.g, 0.f, 1.f) * 255.0f),
              uint8_t(std::clamp(colour.b, 0.f, 1.f) * 255.0f), 255);
}

void Renderer::render()
{
    /*for (size_t i = 0; i < REFRACTAL_MAX_MESH; i++)
    {
        if (buffers[i].is_free && !buffers[i].markDirty)
            continue;
        buffers[i].markDirty = false;
        buffers[i].mesh.vertices.reserve(buffers[i].mesh.original_vertices.size());
        // Scale the vertices by the maximum coordinate value
        for (size_t j = 0; j < buffers[i].mesh.original_vertices.size() - 1; j++)
        {

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), buffers[i].mesh.position);
            transform *= glm::mat4_cast(glm::quat(glm::vec4(buffers[i].mesh.rotation, 1.0f)));
            transform = glm::scale(transform, glm::vec3(1.0f));
            glm::vec3 point_0;
            glm::vec3 point_1;
            glm::vec3 point_2;
            {
                glm::vec4 v_transformed = transform * glm::vec4(buffers[i].mesh.original_vertices[j].point_0, 1.0f);
                glm::vec3 v_final = glm::vec3(v_transformed) / v_transformed.w;
            }
            {
                glm::vec4 v_transformed = transform * glm::vec4(buffers[i].mesh.original_vertices[j].point_1, 1.0f);
                glm::vec3 v_final = glm::vec3(v_transformed) / v_transformed.w;
            }
            {
                glm::vec4 v_transformed = transform * glm::vec4(buffers[i].mesh.original_vertices[j].point_2, 1.0f);
                glm::vec3 v_final = glm::vec3(v_transformed) / v_transformed.w;
            }
        }
    }
    */
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
