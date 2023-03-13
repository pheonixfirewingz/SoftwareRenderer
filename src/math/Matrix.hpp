#pragma once
#include <math/Matrix.hpp>
#include <cmath>
#include <cstdint>

template<typename T> class Matrix4x4
{
  protected:
    T matrix[16];
    Matrix4x4(T matrix_in[16])
        : matrix(matrix_in)
    {
    }
  public:
    static Matrix4x4<T> makeIdentityMatrix() noexcept
    {
        return Matrix4x4<T>(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    }

    Matrix4x4<T> addPosition(Vector3<T> pos) noexcept
    {

    }
};