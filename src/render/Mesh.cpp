#include "Mesh.hpp"
#include <util/Math.hpp>

bool RefractalMesh::hasHit(const Ray &ray, RayHitInfomation &info)
{
    bool hit_tri = false;
    size_t triangles_size = vertices.size();
    float closest_distance = std::numeric_limits<float>::max();
    glm::vec2 closest_barry_pos = {0, 0};
    size_t closest_hit_index = 0;

    for (size_t i = triangles_size - 1; i != 0; --i)
    {
        float distance = 0;
        glm::vec2 barry_pos;
        if (glm::intersectRayTriangle(
                ray.origin(), ray.dir(),
                glm::vec3(vertices[i].point_0.x, vertices[i].point_0.y, vertices[i].point_0.z) / vertices[i].point_0.w,
                glm::vec3(vertices[i].point_1.x, vertices[i].point_1.y, vertices[i].point_1.z) / vertices[i].point_1.w,
                glm::vec3(vertices[i].point_2.x, vertices[i].point_2.y, vertices[i].point_2.z) / vertices[i].point_2.w,
                barry_pos, distance))
        {
            if (distance < closest_distance)
            {
                hit_tri = true;
                closest_distance = distance;
                closest_barry_pos = barry_pos;
                closest_hit_index = i;
            }
        }
    }

    if (hit_tri)
    {
        info.distance = closest_distance;
        info.barry_pos = closest_barry_pos;
        info.colour = vertices[closest_hit_index].colour;
    }

    return hit_tri;
}
