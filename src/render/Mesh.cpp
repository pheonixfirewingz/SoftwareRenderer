#include "Mesh.hpp"

bool RefractalMesh::hasHit(const Ray &ray)
{
    bool hit_tri = false;
    long triangles_size = vertices.size();
    for (long i = 0; i < triangles_size; ++i)
        if (vertices[i].hasHit(ray))
            hit_tri = true;
    return hit_tri;
}

bool RefractalTriangle::hasHit(const Ray &ray)
{
    float u, v, t_temp = 0.0f;

    glm::vec3 pvec = glm::cross(ray.dir(), edge_2);
    float det = glm::dot(edge_1, pvec);
    if (det == 0.0f)
        return false;
    float inv_det = 1.0f / det;
    glm::vec3 tvec = ray.origin() - point_0;
    u = glm::dot(tvec, pvec) * inv_det;
    if (u < 0.0f || u > 1.0f)
        return false;
    glm::vec3 qvec = glm::cross(tvec, edge_1);
    v = glm::dot(ray.dir(), qvec) * inv_det;
    if (v < 0.0f || u + v > 1.0f)
        return false;
    t_temp = glm::dot(edge_2, qvec) * inv_det;
    if (t_temp < std::numeric_limits<float>::max() && t_temp > std::numeric_limits<float>::epsilon())
        return true;
    return false;
}
