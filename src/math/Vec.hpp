#pragma once
#include <cmath>
#include <cstdint>

template<typename T> class Vector2
{
  protected:
    T x_, y_;

  public:
    Vector2(T x, T y)
        : x_(x)
        , y_(y)
    {
    }

    Vector2(T f)
        : x_(f)
        , y_(f)
    {
    }

    T x() const noexcept
    {
        return x_;
    }

    T y() const noexcept
    {
        return y_;
    }

    void addX(T v) noexcept
    {
        x_ += v;
    }

    void addY(T v) noexcept
    {
        y_ += v;
    }

    Vector2 operator-() const
    {
        return Vector2(-x_, -y_);
    }

    Vector2 &operator+=(const Vector2 &v)
    {
        x_ += v.x();
        y_ += v.y();
        return *this;
    }

    Vector2 &operator*=(const Vector2 &v)
    {
        x_ *= v.x();
        y_ *= v.y();
        return *this;
    }

    Vector2 &operator/=(const T t)
    {
        return *this *= 1 / t;
    }

    T length() const
    {
        return std::sqrt(length_squared());
    }

    T length_squared() const
    {
        return x_ * x_ + y_ * y_;
    }

    bool near_zero() const
    {
        constexpr auto s = 1e-8;
        return (std::fabs(x_) < s) && (std::fabs(y_) < s);
    }

    friend Vector2<T> operator+(const Vector2<T> &u, const Vector2<T> &v)
    {
        return Vector2<T>(u.x_ + v.x_, u.y_ + v.y_);
    }

    friend Vector2<T> operator-(const Vector2<T> &u, const Vector2<T> &v)
    {
        return Vector2<T>(u.x_ - v.x_, u.y_ - v.y_);
    }

    friend Vector2<T> operator*(const Vector2<T> &u, const Vector2<T> &v)
    {
        return Vector2<T>(u.x_ * v.x_, u.y_ * v.y_);
    }

    friend Vector2<T> operator/(const Vector2<T> &u, const Vector2<T> &v)
    {
        return Vector2<T>(u.x_ / v.x_, u.y_ / v.y_);
    }

    friend Vector2<T> operator*(T t, const Vector2<T> &v)
    {
        return Vector2<T>(t * v.x_, t * v.y_);
    }

    friend Vector2<T> operator*(const Vector2<T> &v, T t)
    {
        return t * v;
    }

    friend Vector2<T> operator/(Vector2<T> v, T t)
    {
        return (1 / t) * v;
    }

    friend T dot(const Vector2<T> &u, const Vector2<T> &v)
    {
        return u.x_ * v.x_ + u.y_ * v.y_ + u.z_ * v.z_;
    }

    friend Vector2<T> unit_vector(Vector2<T> v)
    {
        return v / v.length();
    }

    friend Vector2<T> reflect(const Vector2<T> &v, const Vector2<T> &n)
    {
        return v - 2 * dot(v, n) * n;
    }

    friend Vector2<T> pow(const Vector2<T> &v, const Vector2<T> &n)
    {
        return Vector2<T>(std::pow(v.x_, n.x_), std::pow(v.y_, n.y_));
    }
};

template<typename T> class Vector3
{
    T x_, y_, z_;

  public:
    Vector3(T x, T y, T z)
        : x_(x)
        , y_(y)
        , z_(z)
    {
    }

    Vector3(T f)
        : x_(f)
        , y_(f)
        , z_(f)
    {
    }

    T x() const noexcept
    {
        return x_;
    }

    T y() const noexcept
    {
        return y_;
    }

    T z() const noexcept
    {
        return z_;
    }

    void addX(T v) noexcept
    {
        x_ += v;
    }

    void addY(T v) noexcept
    {
        y_ += v;
    }

    void addZ(T v) noexcept
    {
        z_ += v;
    }

    Vector3<T> operator-() const
    {
        return Vector3<T>(-x_, -y_, -z_);
    }

    Vector3<T> &operator+=(const Vector3<T> &v)
    {
        x_ += v.x();
        y_ += v.y();
        z_ += v.z();
        return *this;
    }

    Vector3<T> &operator*=(const Vector3<T> &v)
    {
        x_ *= v.x();
        y_ *= v.y();
        z_ *= v.z();
        return *this;
    }

    Vector3<T> &operator/=(const T t)
    {
        return *this *= 1 / t;
    }

    T length() const
    {
        return std::sqrt(length_squared());
    }

    T length_squared() const
    {
        return x_ * x_ + y_ * y_ + z_ * z_;
    }

    bool near_zero() const
    {
        constexpr auto s = 1e-8;
        return (std::fabs(x_) < s) && (std::fabs(y_) < s) && (std::fabs(z_) < s);
    }

    friend Vector3<T> operator+(const Vector3<T> &u, const Vector3<T> &v)
    {
        return Vector3<T>(u.x_ + v.x_, u.y_ + v.y_, u.z_ + v.z_);
    }

    friend Vector3<T> operator-(const Vector3<T> &u, const Vector3<T> &v)
    {
        return Vector3<T>(u.x_ - v.x_, u.y_ - v.y_, u.z_ - v.z_);
    }

    friend Vector3<T> operator*(const Vector3<T> &u, const Vector3<T> &v)
    {
        return Vector3<T>(u.x_ * v.x_, u.y_ * v.y_, u.z_ * v.z_);
    }

    friend Vector3<T> operator/(const Vector3<T> &u, const Vector3<T> &v)
    {
        return Vector3<T>(u.x_ / v.x_, u.y_ / v.y_, u.z_ / v.z_);
    }

    friend Vector3<T> operator*(T t, const Vector3<T> &v)
    {
        return Vector3<T>(t * v.x_, t * v.y_, t * v.z_);
    }

    friend Vector3<T> operator*(const Vector3<T> &v, T t)
    {
        return t * v;
    }

    friend Vector3<T> operator/(Vector3<T> v, T t)
    {
        return (1 / t) * v;
    }

    friend T dot(const Vector3<T> &u, const Vector3<T> &v)
    {
        return u.x_ * v.x_ + u.y_ * v.y_ + u.z_ * v.z_;
    }

    friend Vector3<T> unit_vector(Vector3<T> v)
    {
        return v / v.length();
    }

    friend Vector3<T> reflect(const Vector3<T> &v, const Vector3<T> &n)
    {
        return v - 2 * dot(v, n) * n;
    }

    friend Vector3<T> pow(const Vector3<T> &v, const Vector3<T> &n)
    {
        return Vector3<T>(std::pow(v.x_, n.x_), std::pow(v.y_, n.y_), std::pow(v.z_, n.z_));
    }
};

template<typename T> class Vector4
{
    T x_, y_, z_, w_;

  public:
    Vector4(T x, T y, T z, T w)
        : x_(x)
        , y_(y)
        , z_(z)
        , w_(w)
    {
    }

    Vector4(T f)
        : x_(f)
        , y_(f)
        , z_(f)
        , w_(f)
    {
    }

    T x() const noexcept
    {
        return x_;
    }

    T y() const noexcept
    {
        return y_;
    }

    T z() const noexcept
    {
        return z_;
    }

    T w() const noexcept
    {
        return w_;
    }

    void addX(T v) noexcept
    {
        x_ += v;
    }

    void addY(T v) noexcept
    {
        y_ += v;
    }

    void addZ(T v) noexcept
    {
        z_ += v;
    }

    void addW(T v) noexcept
    {
        w_ += v;
    }

    Vector4<T> operator-() const
    {
        return Vector4<T>(-x_, -y_, -z_, -w_);
    }

    Vector4<T> &operator+=(const Vector4<T> &v)
    {
        x_ += v.x();
        y_ += v.y();
        z_ += v.z();
        w_ += v.w();
        return *this;
    }

    Vector4<T> &operator*=(const Vector4<T> &v)
    {
        x_ *= v.x();
        y_ *= v.y();
        z_ *= v.z();
        w_ *= v.w();
        return *this;
    }

    Vector4<T> &operator/=(const T t)
    {
        return *this *= 1 / t;
    }

    T length() const
    {
        return std::sqrt(length_squared());
    }

    T length_squared() const
    {
        return x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_;
    }

    bool near_zero() const
    {
        constexpr auto s = 1e-8;
        return (std::fabs(x_) < s) && (std::fabs(y_) < s) && (std::fabs(z_) < s) && (std::fabs(w_) < s);
    }

    friend Vector4<T> operator+(const Vector4<T> &u, const Vector4<T> &v)
    {
        return Vector4<T>(u.x_ + v.x_, u.y_ + v.y_, u.z_ + v.z_, u.w_ + v.w_);
    }

    friend Vector4<T> operator-(const Vector4<T> &u, const Vector4<T> &v)
    {
        return Vector4<T>(u.x_ - v.x_, u.y_ - v.y_, u.z_ - v.z_, u.w_ - v.w_);
    }

    friend Vector4<T> operator*(const Vector4<T> &u, const Vector4<T> &v)
    {
        return Vector4<T>(u.x_ * v.x_, u.y_ * v.y_, u.z_ * v.z_, u.w_ * v.w_);
    }

    friend Vector4<T> operator/(const Vector4<T> &u, const Vector4<T> &v)
    {
        return Vector4<T>(u.x_ / v.x_, u.y_ / v.y_, u.z_ / v.z_, u.w_ / v.w_);
    }

    friend Vector4<T> operator*(T t, const Vector4<T> &v)
    {
        return Vector4<T>(t * v.x_, t * v.y_, t * v.z_, t * v.w_);
    }

    friend Vector4<T> operator*(const Vector4<T> &v, T t)
    {
        return t * v;
    }

    friend Vector4<T> operator/(Vector4<T> v, T t)
    {
        return (1 / t) * v;
    }

    friend T dot(const Vector4<T> &u, const Vector4<T> &v)
    {
        return u.x_ * v.x_ + u.y_ * v.y_ + u.z_ * v.z_ + u.w_ * v.w_;
    }

    friend Vector4<T> unit_vector(Vector4<T> v)
    {
        return v / v.length();
    }

    friend Vector4<T> reflect(const Vector4<T> &v, const Vector4<T> &n)
    {
        return v - 2 * dot(v, n) * n;
    }

    friend Vector4<T> pow(const Vector4<T> &v, const Vector4<T> &n)
    {
        return Vector4<T>(std::pow(v.x_, n.x_), std::pow(v.y_, n.y_), std::pow(v.z_, n.z_), std::pow(v.w_, n.w_));
    }
};

using Vec2 = Vector2<double>;
using Vec3 = Vector3<double>;
using Vec4 = Vector4<double>;
using Point = Vector3<double>;
using Colour = Vector4<double>;