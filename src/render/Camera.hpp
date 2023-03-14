#pragma once
#include <App.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <math/Vec.hpp>
#include <render/Ray.hpp>
template<typename T> class Camera
{
    Vector3<T> position;
    Vector4<T> rotation;
    Vector3<T> scale;

  public:
    Camera()
        : position(0, 0, 0)
        , rotation(0, 1, 0, 0)
        , scale(1, 1, 1)
    {
    }

    void update(App *app)
    {
        if (app->IsKeyDown(GLFW_KEY_W))
            position.addZ(0.01);
        if (app->IsKeyDown(GLFW_KEY_S))
            position.addZ(-0.01);
        if (app->IsKeyDown(GLFW_KEY_A))
            position.addX(0.01);
        if (app->IsKeyDown(GLFW_KEY_D))
            position.addX(-0.01);
        if (app->IsKeyDown(GLFW_KEY_SPACE))
            position.addY(0.01);
        if (app->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
            position.addY(-0.01);
    }
};