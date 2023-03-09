#pragma once
#include "Sse3.h"

class Vector3
{
    float vec[3];
  public:
    Vector3(float x, float y, float z)
    {
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
    }


    float x() const
    {
        return vec[0];
    }

    float y() const
    {
        return vec[1];
    }

    float z() const
    {
        return vec[0];
    }
};

class SSE3Vector3
{
    __m128 vec;

  public:
    SSE3Vector3(Vector3 in)
    {
        vec = _mm_set_ps(0.0f, in.z(), in.y(), in.x());
    }

    SSE3Vector3(const __m128 &data)
    {
        vec = data;
    }

    Vector3 getPVec()
    {
        return Vector3(x(), y(), z());
    }

    __m128 data() const
    {
        return vec;
    }

    SSE3Vector3 operator+(const SSE3Vector3 &other) const
    {
        return SSE3Vector3(_mm_add_ps(vec, other.vec));
    }

    SSE3Vector3 operator-(const SSE3Vector3 &other) const
    {
        return SSE3Vector3(_mm_sub_ps(vec, other.vec));
    }

    SSE3Vector3 operator*(const SSE3Vector3 &other) const
    {
        return SSE3Vector3(_mm_mul_ps(vec, other.vec));
    }

    SSE3Vector3 operator*(float scalar) const
    {
        __m128 scalar_data = _mm_set1_ps(scalar);
        return SSE3Vector3(_mm_mul_ps(vec, scalar_data));
    }

    SSE3Vector3 operator/(float scalar) const
    {
        __m128 scalar_data = _mm_set1_ps(scalar);
        return SSE3Vector3(_mm_div_ps(vec, scalar_data));
    }

    float x() const
    {
        return _mm_cvtss_f32(vec);
    }

    float y() const
    {
        __m128 shuf = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1, 1, 1, 1));
        return _mm_cvtss_f32(shuf);
    }

    float z() const
    {
        __m128 shuf = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2, 2, 2, 2));
        return _mm_cvtss_f32(shuf);
    }
};
