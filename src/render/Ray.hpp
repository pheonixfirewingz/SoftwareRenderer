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

    glm::vec3 src() const
    {
        return _src;
    }
    glm::vec3 dir() const
    {
        return _dir;
    }

    Ray operator-(Ray& ray)
    {
        return Ray(ray._src, glm::vec3(-ray._dir.x,-ray._dir.y,-ray._dir.z));
    }

    glm::vec3 at(float t) const
    {
        return _src + t * _dir;
    }
};
