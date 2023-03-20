#pragma once
#include <util/Math.hpp>
class Ray
{
    glm::vec3 _src;
    glm::vec3 _dir;

  public:
    Ray() = default;
    Ray(const glm::vec3 &src, const glm::vec3 &dir)
        : _src(src)
        , _dir(dir)
    {
    }
    glm::vec3 origin() const noexcept
    {
        return _src;
    }
    glm::vec3 dir() const noexcept
    {
        return _dir;
    }
};

struct RayHitInfomation
{
    float distance = std::numeric_limits<float>::max();
    glm::vec2 barry_pos = {0, 0};
    // temp
    glm::vec3 colour{0, 0, 0};
};
