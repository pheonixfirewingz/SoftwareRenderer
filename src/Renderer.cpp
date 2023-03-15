#include "Renderer.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _OPENMP
#    include <omp.h>
#endif
#include <render/Ray.hpp>
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

void Renderer::setProjectionMatrix(glm::mat4x4 mat)
{
    proj_matrix = mat;
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
    buffers[id].vertices_transformed.clear();
#ifdef _OPENMP
#    pragma omp parallel for
#endif
    for (Vertex &v : buffers[id].vertices)
    {
        Vertex nv = v;
        glm::vec4 new_pos = proj_matrix * (glm::rotate(glm::translate(glm::mat4x4(1.0f), buffers[id].position),
                                                       glm::radians(45.0f), buffers[id].rotation) *
                                           glm::vec4(v.position, 1.0f));
        nv.position = glm::vec3(new_pos.x, new_pos.y, new_pos.z);
        buffers[id].vertices_transformed.push_back(nv);
    }
}

void Renderer::setMeshRotation(const uint64_t id, glm::vec3 new_rotation)
{
    buffers[id].rotation = new_rotation;
    buffers[id].vertices_transformed.clear();
#ifdef _OPENMP
#    pragma omp parallel for
#endif
    for (Vertex &v : buffers[id].vertices)
    {
        Vertex nv = v;
        glm::vec4 new_pos = proj_matrix * (glm::rotate(glm::translate(glm::mat4x4(1.0f), buffers[id].position),
                                                       glm::radians(45.0f), buffers[id].rotation) *
                                           glm::vec4(v.position, 1.0f));
        nv.position = glm::vec3(new_pos.x, new_pos.y, new_pos.z);
        buffers[id].vertices_transformed.push_back(nv);
    }
}

void Renderer::transferData(const uint64_t id, const uint8_t type, const size_t buffer_size, void *data)
{
    switch (type)
    {
    case REFRACTAL_VERTEX_BUFFER: {
        buffers[id].vertices.reserve(buffer_size);
        memcpy(buffers[id].vertices.data(), data, buffer_size);
    }
    break;
    case REFRACTAL_INDEX_BUFFER: {
        buffers[id].indices.reserve(buffer_size);
        memcpy(buffers[id].indices.data(), data, buffer_size);
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
    buffers[id].indices.clear();
}

void Renderer::processPixel(const uint64_t x, const uint64_t y)
{
    glm::vec2 uv((float(x) / internal_width), (float(y) / internal_height));
    glm::vec4 colour(1, 1, 1, 1); // colour is initalized to white(solid) aka 1,1,1,1;

    for(uint8_t bounce = 1; bounce < 4; bounce++)
    {
        glm::vec4 ray_colour(0, 0, 0, 0);
        glm::vec3 src = {x,y,0};
        glm::vec3 dir = {rand() % UINT32_MAX,rand() % UINT32_MAX,-1.0};
        float distance = 100.f;
        for(size_t i = 0; i < (REFRACTAL_MAX_MESH - 1); i++)
        {
            const Mesh& mesh = buffers[i];
            if(mesh.is_free)
                continue;
            for(size_t l = 3; l < mesh.indices.size() - 1; l += 3)
            {
                if(glm::intersectRayTriangle(src,dir,mesh.vertices[mesh.indices[l - 2]].position,mesh.vertices[mesh.indices[l - 1]].position,mesh.vertices[mesh.indices[l]].position,uv,distance))
                    ray_colour += glm::vec4(0.f,0.f,0.f,1.f);
                else
                    ray_colour += glm::vec4(.1f,.1f,.1f,1.f);
            }
        }
        colour += ray_colour / (float(bounce) / .5f);
    }

    screen_data[y * internal_width + x] = {uint8_t(colour.x * 255.0f), uint8_t(colour.y * 255.0f),
                                           uint8_t(colour.z * 255.0f), uint8_t(colour.w * 255.0f)};
}

void Renderer::render()
{
#ifndef _OPENMP
#    pragma omp parallel for
#endif
    for (int64_t j = 0; j < internal_height; j++)
        for (int64_t i = 0; i < internal_width; i++)
            processPixel(i, j);
}

const void *Renderer::getScreenData() const noexcept
{
    return (const void *)screen_data;
}
