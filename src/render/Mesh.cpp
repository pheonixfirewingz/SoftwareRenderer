#include "Mesh.hpp"

bool RefractalMesh::hasHit(const Ray &ray, float& distance)
{
    bool hit_tri = false;
    size_t triangles_size = vertices.size();
    float distance_ = std::numeric_limits<float>::epsilon();
    for (size_t i = 0; i < triangles_size; ++i)
        if (vertices[i].hasHit(ray, distance_))
        {
            hit_tri = true;
            if(distance < distance_)
                distance = distance_;
        }
    return hit_tri;
}

bool RefractalTriangle::hasHit(const Ray &ray, float &distance)
{
    float u, v;

    glm::vec3 pvec = glm::cross(ray.dir(), edge_2);
    float det = glm::dot(edge_1, pvec);
    if (det == 0.0f)
        return false;
    float inv_det = 1.0f / det;
    glm::vec3 tvec = ray.origin() - glm::vec3(point_0.x,point_0.y,point_0.z);
    u = glm::dot(tvec, pvec) * inv_det;
    if (u < 0.0f || u > 1.0f)
        return false;
    glm::vec3 qvec = glm::cross(tvec, edge_1);
    v = glm::dot(ray.dir(), qvec) * inv_det;
    if (v < 0.0f || u + v > 1.0f)
        return false;
    distance = glm::dot(edge_2, qvec) * inv_det;
    if (distance < std::numeric_limits<float>::max() && distance > std::numeric_limits<float>::epsilon())
        return true;
    return false;
}
