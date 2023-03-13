#pragma once
#include <math/Vec.hpp>
#include <render/Ray.hpp>
#include <glfw/glfw3.h>
#include <App.h>
template<typename T>
class Camera
{
    Vector3<T> position;
    Vector4<T> rotation;
    Vector3<T> scale;
    public:
    Camera()
          : position(0, 0, 0)
          , rotation(0,1,0,0)
          , scale(1, 1, 1)
      {
      }

    void update()
    {
        App *app = App::getActiveApp();
        if (app->IsKeyDown(GLFW_KEY_W))
        {
            position.z += 0.01f;
        }
        if (app->IsKeyDown(GLFW_KEY_S))
        {
            position.z -= 0.01f;
        }
        if (app->IsKeyDown(GLFW_KEY_A))
        {
            position.x += 0.01f;
        }
        if (app->IsKeyDown(GLFW_KEY_D))
        {
            position.x -= 0.01f;
        }
        if (app->IsKeyDown(GLFW_KEY_SPACE))
        {
            position.y += 0.01f;
        }
        if (app->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
        {
            position.y  -= 0.01f;
        }
    }
};