#include "Renderer.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#if defined(_OPENMP) && !defined(WITH_DEBUG)
#    include <omp.h>
#endif
#include <inttypes.h>
#include <render/Ray.hpp>
#include <util/Math.hpp>
#include <util/Util.hpp>

Renderer::Renderer(int width, int height)
    : internal_width(width)
    , internal_height(height)
{
    proj = glm::perspective(glm::radians(90.0f), float(width) / float(height), 0.1f, 100.0f);
    screen_data = new Pixel[width * height];
}

void Renderer::resizeScreen(int width, int height)
{
    proj = glm::perspective(glm::radians(90.0f), float(width) / float(height), 0.1f, 100.0f);
    screen_data =
        static_cast<Pixel *>(realloc(screen_data, static_cast<size_t>(internal_width = width) *
                                                      static_cast<size_t>(internal_height = height) * sizeof(Pixel)));
}

uint64_t Renderer::genLight()
{
    for (uint64_t i = 0; i < REFRACTAL_MAX_LIGHTS; i++)
    {
        if (lights[i].is_free)
        {
            lights[i].set();
            return i;
        }
    }
    return REFRACTAL_MAX_LIGHTS;
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

void Renderer::transferData(const uint64_t id, const uint8_t type, const uint8_t format, const size_t buffer_size,
                            const void *data)
{
    if (buffer_size == 0)
        return;

    switch (type)
    {
    case REFRACTAL_VERTEX_BUFFER: {
        size_t num_vertices = buffer_size / getVertexFormatSize(format);
        switch (format)
        {
        case REFRACTAL_VERTEX_FORMAT_XYZ: {
            std::vector<glm::vec3> vertices;
            const float *buffer_data = static_cast<const float *>(data);
            for (size_t i = 0; i < num_vertices; i++)
            {
                float x = *(buffer_data++);
                float y = *(buffer_data++);
                float z = *(buffer_data++);
                vertices.push_back(glm::vec4(x, y, z, 1.0f));
            }

            buffers[id].original_vertices.resize(num_vertices / 3);
            for (size_t i = 0, j = 0; i < num_vertices; i += 3, j++)
            {
                buffers[id].original_vertices[j] =
                    RefractalTriangle(vertices[i], vertices[i + 1], vertices[i + 2], false);
            }
            break;
        }
        case REFRACTAL_VERTEX_FORMAT_XYZ_UV: {
            std::vector<glm::vec3> vertices;
            std::vector<glm::vec2> uvs;
            const float *buffer_data = static_cast<const float *>(data);
            for (size_t i = 0; i < num_vertices; i++)
            {
                float x = *(buffer_data++);
                float y = *(buffer_data++);
                float z = *(buffer_data++);
                float u = *(buffer_data++);
                float v = *(buffer_data++);
                vertices.push_back(glm::vec3(x, y, z));
                uvs.push_back(glm::vec2(u, v));
            }

            buffers[id].original_vertices.resize(num_vertices / 3);
            for (size_t i = 0, j = 0; i < num_vertices; i += 3, j++)
            {
                buffers[id].original_vertices[j] = RefractalTriangle(vertices[i], vertices[i + 1], vertices[i + 2],
                                                                     uvs[i], uvs[i + 1], uvs[i + 2], false);
            }
            break;
        }
        // add cases for other formats as needed
        default:
            break;
        }
        break;
    }
    // add cases for other buffer types as needed
    default:
        break;
    }
}

size_t Renderer::getVertexFormatSize(const uint8_t format) const noexcept
{
    switch (format)
    {
    case REFRACTAL_VERTEX_FORMAT_XYZ:
        return sizeof(float) * 3;
    case REFRACTAL_VERTEX_FORMAT_XYZ_UV:
        return sizeof(float) * 5;
    // add cases for other formats as needed
    default:
        return 0;
    }
}

void Renderer::render()
{
    //clear buffer to clear screen values
    for (uint64_t i = 0; i < internal_width * internal_height; i++)
        screen_data[i] = Pixel(uint8_t(base_colour.r), uint8_t(base_colour.g), uint8_t(base_colour.b), 255);

   //calculate cam matrix
    glm::vec3 forward =
        glm::rotate(glm::quat(glm::vec3(glm::radians(viewport_rotation.y), glm::radians(viewport_rotation.x),
                                        glm::radians(viewport_rotation.z))),
                    glm::vec3(0.0f, 0.0f, -1.0f));
    glm::mat4 view = glm::lookAt(viewport_position, viewport_position + forward, glm::vec3(0, 1, 0));
#ifdef _OPENMP
#    pragma omp parallel for
#endif
#ifdef ON_WINDOWS
    for (int64_t i = 0; i < REFRACTAL_MAX_MESH; i++)
#else
    for (size_t i = 0; i < REFRACTAL_MAX_MESH; i++)
#endif //  ON_WINDOWS
    {
        if (buffers[i].is_free)
            continue;
        //transform vertex from model space to world space this assumes that model data is normalized to between -1.0 & 1.0
        buffers[i].vertices.clear();
        buffers[i].vertices.reserve(buffers[i].original_vertices.size());
        for (size_t j = 0; j < buffers[i].original_vertices.size(); j++)
        {
            //calculate the model matrix
            glm::mat4 transform = glm::mat4_cast(glm::quat(glm::vec4(buffers[i].rotation, 1.0f)));
            transform *= glm::translate(glm::mat4(1.0f), buffers[i].position);
            transform = glm::scale(transform, glm::vec3(1.f));

            //GLSL equivent:  gl_Position = projection * view * model * vec4(aPos, 1.0f);
            glm::vec4 point_0 = proj * view * transform * glm::vec4(buffers[i].original_vertices[j].point_0, 1.0f);
            glm::vec4 point_1 = proj * view * transform * glm::vec4(buffers[i].original_vertices[j].point_1, 1.0f);
            glm::vec4 point_2 = proj * view * transform * glm::vec4(buffers[i].original_vertices[j].point_2, 1.0f);

            buffers[i].vertices.emplace_back(RefractalTriangle(glm::vec3(point_0.x, point_0.y, point_0.z) / point_0.w,
                                                               glm::vec3(point_1.x, point_1.y, point_1.z) / point_1.w,
                                                               glm::vec3(point_2.x, point_2.y, point_2.z) / point_2.w,
                                                               false));
        }
    }

    //fragment shader stage
#if defined(_OPENMP)
#    pragma omp parallel for
#endif
    for (int64_t y = 0; y < internal_height; y++)
        for (int64_t x = 0; x < internal_width; x++)
        {
            // fragment coordinates
            glm::vec2 coord =
                glm::vec2((float)x / (float)internal_width, (float)y / (float)internal_height) * 2.0f - 1.0f;
            // base clour if 4th elememt is set not set to 0 then the pixel data is updated no point updating the buffer
            // if nothing is there to add to the image
            glm::vec4 colour(0, 0, 0, 0);
            // path trace to get pixel colour of the scene


            //Note: the final colour needs to be between 0.0 and 1.0 to be able to format it to the correct colour range correctly
            if (colour.a != 0)
                screen_data[x + y * internal_width] = Pixel(uint8_t(colour.r * 255.0f), uint8_t(colour.g * 255.0f),
                                                            uint8_t(colour.b * 255.0f), uint8_t(colour.a * 255.0f));
        }
}

bool Renderer::MeshRegister::hasHit(const Ray &ray, RayHitInfomation &info) noexcept
{
    bool hit_tri = false;
    size_t triangles_size = vertices.size();
    for (size_t i = triangles_size - 1; i != 0; --i)
    {
        float distance = 0;
        glm::vec2 barry;
        if (glm::intersectRayTriangle(ray.src, ray.dir, vertices[i].point_0, vertices[i].point_1, vertices[i].point_2,
                                      barry, distance))
        {
            if (distance < info.distance)
            {
                hit_tri = true;
                info.triangle_index = i;
                info.distance = distance;
                glm::vec3 p0 = vertices[i].point_0;
                glm::vec3 p1 = vertices[i].point_1;
                glm::vec3 p2 = vertices[i].point_2;
                info.intersection_point = p0 + barry.x * (p1 - p0) + barry.y * (p2 - p0);
            }
        }
    }
    return hit_tri;
}