#pragma once
#include "Sse3.h"
#include "Vector3.hpp"
#include "Quaternion.hpp"
#include <cstring>
class Matrix4x4
{
    float m[4][4] __attribute__((aligned(16)));

  public:
    Matrix4x4()
    {
        memset(m, 0, sizeof(float) * 16);
        m[0][0] = 1.0f;
        m[1][1] = 1.0f;
        m[2][2] = 1.0f;
        m[3][3] = 1.0f;
    }
    Matrix4x4(const float *data)
    {
        _mm_storeu_ps(&m[0][0], _mm_loadu_ps(&data[0]));
        _mm_storeu_ps(&m[1][0], _mm_loadu_ps(&data[4]));
        _mm_storeu_ps(&m[2][0], _mm_loadu_ps(&data[8]));
        _mm_storeu_ps(&m[3][0], _mm_loadu_ps(&data[12]));
    }
    __m128 operator[](int i) const
    {
        return *(__m128 *)m[i];
    }
    __m128 &operator[](int i)
    {
        return *(__m128 *)m[i];
    }

    Matrix4x4 cross(const Matrix4x4 &other) const
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i)
        {
            __m128 a = (*this)[i];
            __m128 b = other[i];

            __m128 a_yzx = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 b_yzx = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1));

            __m128 prod1 = _mm_mul_ps(a, b_yzx);
            __m128 prod2 = _mm_mul_ps(a_yzx, b);

            __m128 sub = _mm_sub_ps(prod1, prod2);

            result[i] = sub;
        }
        return result;
    }

    Matrix4x4 makeTransformMatrix(const SSE3Vector3 &position, const SSE3Quaternion &rotation, const SSE3Vector3 &scale)
    {
        Matrix4x4 matrix;

        __m128 rotationData = rotation.data();
        __m128 positionData = position.data();
        __m128 scaleData = scale.data();
        __m128 wxyz = _mm_shuffle_ps(rotationData, rotationData, _MM_SHUFFLE(3, 0, 1, 2));
        __m128 yxwz = _mm_shuffle_ps(rotationData, rotationData, _MM_SHUFFLE(1, 0, 3, 2));
        __m128 zywx = _mm_shuffle_ps(rotationData, rotationData, _MM_SHUFFLE(2, 3, 0, 1));
        //__m128 negWxyz = _mm_mul_ps(wxyz, _mm_set_ps(-1.0f, 1.0f, 1.0f, 1.0f));
        __m128 negYxwz = _mm_mul_ps(yxwz, _mm_set_ps(1.0f, -1.0f, 1.0f, 1.0f));
        __m128 negZywx = _mm_mul_ps(zywx, _mm_set_ps(1.0f, 1.0f, -1.0f, 1.0f));
        __m128 row0 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(wxyz, wxyz), _mm_mul_ps(yxwz, yxwz)), _mm_mul_ps(zywx, zywx));
        __m128 row1 =
            _mm_add_ps(_mm_sub_ps(_mm_mul_ps(negYxwz, wxyz), _mm_mul_ps(negZywx, yxwz)), _mm_mul_ps(wxyz, zywx));
        __m128 row2 =
            _mm_add_ps(_mm_add_ps(_mm_mul_ps(yxwz, wxyz), _mm_mul_ps(wxyz, negZywx)), _mm_mul_ps(negYxwz, zywx));
        matrix[0] = _mm_mul_ps(row0, scaleData);
        matrix[1] = _mm_mul_ps(row1, scaleData);
        matrix[2] = _mm_mul_ps(row2, scaleData);
        matrix[3] = positionData;
        matrix[0][3] = matrix[1][3] = matrix[2][3] = 0.0f;
        matrix[3][3] = 1.0f;

        return matrix;
    }
};

inline Matrix4x4 operator*(const Matrix4x4 &a, const Matrix4x4 &b)
{
    Matrix4x4 c;
    __m128 row[4], tmp1, tmp2, sum;
    for (int i = 0; i < 4; ++i)
    {
        row[0] = a[i];
        row[1] = _mm_shuffle_ps(row[0], row[0], _MM_SHUFFLE(0, 0, 0, 0));
        row[2] = _mm_shuffle_ps(row[0], row[0], _MM_SHUFFLE(1, 1, 1, 1));
        row[3] = _mm_shuffle_ps(row[0], row[0], _MM_SHUFFLE(2, 2, 2, 2));
        tmp1 = _mm_mul_ps(row[1], _mm_shuffle_ps(b[0], b[0], _MM_SHUFFLE(0, 0, 0, 0)));
        tmp2 = _mm_mul_ps(row[2], _mm_shuffle_ps(b[1], b[1], _MM_SHUFFLE(1, 1, 1, 1)));
        tmp1 = _mm_add_ps(tmp1, tmp2);
        tmp2 = _mm_mul_ps(row[3], _mm_shuffle_ps(b[2], b[2], _MM_SHUFFLE(2, 2, 2, 2)));
        sum = _mm_add_ps(tmp1, tmp2);
        sum = _mm_add_ps(sum, _mm_shuffle_ps(sum, sum, _MM_SHUFFLE(1, 0, 3, 2)));
        c[i] = sum;
    }
    return c;
}

inline Matrix4x4 operator-(const Matrix4x4 &a, const Matrix4x4 &b)
{
    Matrix4x4 c;
    for (int i = 0; i < 4; i++)
        c[i] = _mm_sub_ps(a[i], b[i]);
    return c;
}

inline Matrix4x4 operator+(const Matrix4x4 &a, const Matrix4x4 &b)
{
    Matrix4x4 c;
    for (int i = 0; i < 4; i++)
        c[i] = _mm_add_ps(a[i], b[i]);
    return c;
}

inline Matrix4x4 operator/(const Matrix4x4 &a, const Matrix4x4 &b)
{
    Matrix4x4 c;
    for (int i = 0; i < 4; i++)
    {
        __m128 a_row = a[i];
        __m128 b_row = b[i];
        __m128 result = _mm_div_ps(a_row, b_row);
        c[i] = result;
    }
    return c;
}