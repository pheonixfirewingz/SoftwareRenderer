#pragma once
#include <render/Ray.hpp>
#include <vector>

struct RefractalTriangle
{
    glm::vec4 point_0;
    glm::vec4 point_1;
    glm::vec4 point_2;
    glm::vec3 colour;

    RefractalTriangle() = default;
    RefractalTriangle(glm::vec4 point_0_in, glm::vec4 point_1_in, glm::vec4 point_2_in, bool normalize = true)
        : point_0(normalize ? glm::normalize(point_0_in) : point_0_in)
        , point_1(normalize ? glm::normalize(point_1_in) : point_1_in)
        , point_2(normalize ? glm::normalize(point_2_in) : point_2_in)
    {
        colour = glm::vec3((float(rand()) / RAND_MAX) + 0.1f, (float(rand()) / RAND_MAX) + 0.1f,
                           (float(rand()) / RAND_MAX) + 0.1f);
    }
};

struct RefractalMesh
{
    std::vector<RefractalTriangle> original_vertices;
    std::vector<RefractalTriangle> vertices;
    glm::vec3 position;
    glm::vec3 rotation;
    bool hasHit(const Ray &ray, RayHitInfomation &distance);
};