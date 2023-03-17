#pragma once
#include <render/Ray.hpp>
#include <vector>

struct Hittable
{
    virtual bool hasHit(const Ray &ray, float& distance) = 0;
};

struct RefractalTriangle : public Hittable
{
    glm::vec3 point_0;
    glm::vec3 point_1;
    glm::vec3 point_2;
    glm::vec3 edge_1;
    glm::vec3 edge_2;

    RefractalTriangle() = default;
    RefractalTriangle(glm::vec3 point_0_in, glm::vec3 point_1_in, glm::vec3 point_2_in)
        : point_0(point_0_in)
        , point_1(point_1_in)
        , point_2(point_2_in)
    {
        edge_1 = point_1 - point_0;
        edge_2 = point_2 - point_0;
    }
    virtual bool hasHit(const Ray &ray,float& distance) final override;
};

struct RefractalMesh : public Hittable
{
    std::vector<RefractalTriangle> original_vertices;
    std::vector<RefractalTriangle> vertices;
    glm::vec3 position;
    glm::vec3 rotation;
    virtual bool hasHit(const Ray &ray, float& distance) final override;
};