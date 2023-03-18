#include "Mesh.hpp"
#include <util/Math.hpp>

bool RefractalMesh::hasHit(const Ray &ray, float &distance)
{
    bool hit_tri = false;
    size_t triangles_size = vertices.size();
    float distance_ = 0;
    for (size_t i = 0; i < triangles_size; ++i)
    {   
        glm::vec2 barry_pos;
        if (glm::intersectRayTriangle(ray.origin(), ray.dir(), glm::vec3(vertices[i].point_0.x, vertices[i].point_0.y, vertices[i].point_0.z) / vertices[i].point_0.w,
                                     glm::vec3(vertices[i].point_1.x, vertices[i].point_1.y, vertices[i].point_1.z) / vertices[i].point_1.w,
                                     glm::vec3(vertices[i].point_2.x, vertices[i].point_2.y, vertices[i].point_2.z)/ vertices[i].point_2.w, barry_pos, distance))
        {
            hit_tri = true;
            if (distance < distance_)
                distance = distance_;
        }
    }
    return hit_tri;
}
