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
    // clear buffer to clear screen values
    for (uint64_t pixel_index = 0; pixel_index < internal_width * internal_height; pixel_index++)
        screen_data[pixel_index] = Pixel(uint8_t(base_colour.r), uint8_t(base_colour.g), uint8_t(base_colour.b), 255);

    {
        // calculate cam matrix
        glm::vec3 forward =
            glm::rotate(glm::quat(glm::vec3(glm::radians(viewport_rotation.y), glm::radians(viewport_rotation.x),
                                            glm::radians(viewport_rotation.z))),
                        glm::vec3(0.0f, 0.0f, -1.0f));
        glm::mat4 view = glm::lookAt(viewport_position, viewport_position + forward, glm::vec3(0, 1, 0));
#ifdef _OPENMP
#    pragma omp parallel for
#endif
        for (int64_t i = 0; i < REFRACTAL_MAX_MESH; i++)
        {
            if (buffers[i].is_free)
                continue;
            // transform vertex from model space to world space this assumes that model data is normalized to between
            // -1.0 & 1.0
            buffers[i].vertices.clear();
            buffers[i].vertices.reserve(buffers[i].original_vertices.size());
            for (size_t j = 0; j < buffers[i].original_vertices.size(); j++)
            {
                // calculate the model matrix
                glm::mat4 transform = glm::mat4(1.0f);
                transform = glm::rotate(transform, glm::radians(buffers[i].rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                transform = glm::rotate(transform, glm::radians(buffers[i].rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
                transform = glm::rotate(transform, glm::radians(buffers[i].rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
                transform = glm::translate(transform, buffers[i].position);
                transform = glm::scale(transform, glm::vec3(1.0f));
                // GLSL equivent:  gl_Position = projection * view * model * vec4(aPos, 1.0f);
                glm::vec4 point_0 = proj * view * transform * glm::vec4(buffers[i].original_vertices[j].point_0, 1.0f);
                glm::vec4 point_1 = proj * view * transform * glm::vec4(buffers[i].original_vertices[j].point_1, 1.0f);
                glm::vec4 point_2 = proj * view * transform * glm::vec4(buffers[i].original_vertices[j].point_2, 1.0f);

                buffers[i].vertices.emplace_back(
                    RefractalTriangle(glm::vec3(point_0.x, point_0.y, point_0.z) / point_0.w,
                                      glm::vec3(point_1.x, point_1.y, point_1.z) / point_1.w,
                                      glm::vec3(point_2.x, point_2.y, point_2.z) / point_2.w, false));
            }
        }

#ifdef _OPENMP
#    pragma omp parallel for
#endif
        for (int64_t i = 0; i < REFRACTAL_MAX_LIGHTS; i++)
        {
            if (lights[i].is_free)
                continue;
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::rotate(transform, glm::radians(lights[i].rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            transform = glm::rotate(transform, glm::radians(lights[i].rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            transform = glm::rotate(transform, glm::radians(lights[i].rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            transform = glm::translate(transform, lights[i].user_position);
            glm::vec4 point = proj * view * transform * glm::vec4(lights[i].user_position, 1.0f);
            lights[i].position = glm::vec3(point.x, point.y, point.z) / point.w;
        }
    }
    // fragment shader stage
#if defined(_OPENMP)
#    pragma omp parallel for
#endif
    for (int64_t y = 0; y < internal_height; y++)
        for (int64_t x = 0; x < internal_width; x++)
        {
            // fragment coordinates
            glm::vec2 coord =
                glm::vec2((float)x / (float)internal_width, (float)y / (float)internal_height) * 2.0f - 1.0f;
            // find closest mesh
            Ray ray({0, 0, 0}, {coord.x, coord.y, -1.0f});
            RayHitInfomation info{};
            MeshRegister &mesh_hit = buffers[0];
            bool found = false;
            for (MeshRegister &mesh : buffers)
            {
                if (mesh.is_free)
                    continue;
                RayHitInfomation _info{};
                // did ray hit the mesh
                if (mesh.hasHit(ray, _info))
                {
                    if (_info.distance < info.distance)
                    {
                        info = _info;
                        mesh_hit = mesh;
                    }
                    found = true;
                }
            }
            /* path trace to get pixel colour of the scene
                if an intersection was found, calculate pixel colour by casting ray from non free LightRegisters if
               they hit the triangle in info without anything obstructions we need to add the light colour depending on
               the rays distance between the intersection and the light origin
            */
            if (found)
            {
                glm::vec3 colour(0, 0, 0);
                for (const LightRegister &light : lights)
                {
                    if (light.is_free)
                        continue;
                    // loop through lights in the scene
                    bool obstructed = false;
                    // cast ray from light origin to intersection point
                    Ray light_ray(light.position, ray.src + (std::abs(info.distance) * ray.dir));
                    if (mesh_hit.isBlocked(light_ray, info.index))
                    {
                        obstructed = true;
                        break;
                    }

                    if (!obstructed)
                    {
                        // if no obstructions between light and intersection point
                        // calculate distance between light origin and intersection point
                        glm::vec3 light_colour = light.colour / (info.distance / light.luminance);
                        float dif = glm::clamp(glm::dot(info.normal, light_colour), 0.f, 1.f);
                        colour += glm::vec3(dif); // add light to pixel colour at intersection point
                    }
                }
                // set pixel colour
                screen_data[x + y * internal_width] =
                    Pixel(uint8_t(colour.r * 255.0f), uint8_t(colour.g * 255.0f), uint8_t(colour.b * 255.0f), 255);
            }
        }
}

bool Renderer::MeshRegister::hasHit(const Ray &ray, RayHitInfomation &info) noexcept
{
    bool hit_tri = false;
    size_t index = 0;
    glm::vec2 bary_{0, 0};
    for (size_t i = vertices.size() - 1; i != 0; --i)
    {
        float distance = 0;
        glm::vec2 bary{0, 0};
        // did ray hit the triangle
        if (glm::intersectRayTriangle(ray.src, ray.dir, vertices[i].point_0, vertices[i].point_1, vertices[i].point_2,
                                      bary, distance))
        {
            if (distance < info.distance)
            {
                hit_tri = true;
                info.distance = distance;
                info.normal = vertices[i].normal;
                index = i;
                bary_ = bary;
            }
        }
    }
    return hit_tri;
}

bool Renderer::MeshRegister::isBlocked(const Ray &ray, const size_t test_triangle_index) noexcept
{
    bool hit_tri = false;
    const float test_dist =
        glm::dot(vertices[test_triangle_index].normal, vertices[test_triangle_index].point_0 - ray.src) /
        glm::dot(ray.dir, vertices[test_triangle_index].normal);
    for (size_t i = 0; i < vertices.size(); i++)
    {
        if (i == test_triangle_index)
            continue;
        const RefractalTriangle &triangle = vertices[i];
        glm::vec2 bary;
        float dist;
        if (glm::intersectRayTriangle(ray.src, ray.dir, triangle.point_0, triangle.point_1, triangle.point_2, bary,
                                      dist))
        {
            if (dist > 0.0f && dist < test_dist)
            {
                hit_tri = true;
                break;
            }
        }
    }
    return hit_tri;
}