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

struct RayHitInfomation
{
    size_t triangle_index = 0;
    float distance = std::numeric_limits<float>::max();
    glm::vec3 intersection_point;
};