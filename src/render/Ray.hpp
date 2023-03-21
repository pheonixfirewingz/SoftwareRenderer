#pragma once
#include <util/Math.hpp>
struct Ray
{
    glm::vec3 src;
    glm::vec3 dir;
    Ray() = default;
    Ray(const glm::vec3 &src_in, const glm::vec3 &dir_in)
        : src(src_in)
        , dir(dir_in)
    {
    }
};

struct RefractalTriangle
{
    glm::vec3 point_0{0.f, 0.f, 0.f};
    glm::vec2 point_0_uv{0.f, 0.f};
    glm::vec3 point_1{0.f, 0.f, 0.f};
    glm::vec2 point_1_uv{0.f, 0.f};
    glm::vec3 point_2{0.f, 0.f, 0.f};
    glm::vec2 point_2_uv{0.f, 0.f};
    RefractalTriangle() = default;
    RefractalTriangle(glm::vec3 point_0_in, glm::vec3 point_1_in, glm::vec3 point_2_in, bool normalize = true)
        : point_0(normalize ? glm::normalize(point_0_in) : point_0_in)
        , point_1(normalize ? glm::normalize(point_1_in) : point_1_in)
        , point_2(normalize ? glm::normalize(point_2_in) : point_2_in)
    {
    }

    RefractalTriangle(glm::vec3 point_0_in, glm::vec3 point_1_in, glm::vec3 point_2_in, glm::vec2 point_0_uv_in,
                      glm::vec2 point_1_uv_in, glm::vec2 point_2_uv_in, bool normalize = true)
        : point_0(normalize ? glm::normalize(point_0_in) : point_0_in)
        , point_0_uv(point_0_uv_in)
        , point_1(normalize ? glm::normalize(point_1_in) : point_1_in)
        , point_1_uv(point_1_uv_in)
        , point_2(normalize ? glm::normalize(point_2_in) : point_2_in)
        , point_2_uv(point_2_uv_in)
    {
    }
};

struct RayHitInfomation
{
    float distance = std::numeric_limits<float>::max();
    size_t triangle_index = 0;
    RefractalTriangle* triangle_hit = nullptr;
};