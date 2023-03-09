#pragma once
#include <math/Vec.hpp>

template<typename T> class Ray
{
    Vector3<T> _src;
    Vector3<T> _dir;

  public:
    Ray() = default;
    Ray(const Vector3<T> &src, const Vector3<T> &dir)
        : _src(src)
        , _dir(dir)
    {
    }

    Vector3<T> src() const
    {
        return _src;
    }
    Vector3<T> dir() const
    {
        return _dir;
    }

    Vector3<T> at(double t) const
    {
        return _src + t * _dir;
    }
};

template<typename T> struct HitInfomation
{
    Vector3<T> normal;
    bool front_face;

    void setNormals(const ray &r, const Vector3<T> &outward_normal)
    {
        front_face = dot<T>(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

template<typename T> struct HitObject
{
    virtual bool didHit(const Ray<T> &ray, const T min, const T max, HitInfomation &hit_info) const = 0;
};