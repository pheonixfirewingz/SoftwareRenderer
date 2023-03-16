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

    glm::vec3 origin() const
    {
        return _src;
    }
    glm::vec3 dir() const
    {
        return _dir;
    }
};
