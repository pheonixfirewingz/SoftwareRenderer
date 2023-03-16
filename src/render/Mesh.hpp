#pragma once
#include <render/Ray.hpp>
#include <vector>

struct Hittable
{
    virtual bool hasHit(const Ray &ray) = 0;
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
        edge_1 = point_1_in - point_0_in;
        edge_2 = point_2_in - point_0_in;
    }
    virtual bool hasHit(const Ray &ray) final override;
};

struct RefractalMesh : public Hittable
{
    std::vector<RefractalTriangle> original_vertices;
    std::vector<RefractalTriangle> vertices;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 colour;
    RefractalMesh()
    {
        colour = glm::vec3((float)rand() / RAND_MAX);
    }
    ~RefractalMesh() = default;
    virtual bool hasHit(const Ray &ray) final override;
};