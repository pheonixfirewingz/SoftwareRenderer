#pragma once
#include <App.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <render/Ray.hpp>
class Camera
{
    glm::vec3 position;
    glm::vec3 rotation;
  public:
    Camera()
        : position(0, 0, 0)
        , rotation(0, 1, 0)
    {
    }

    void update(App *app)
    {
        if (app->IsKeyDown(GLFW_KEY_W))
            position.z += 0.01f;
        if (app->IsKeyDown(GLFW_KEY_S))
            position.z -= 0.01f;
        if (app->IsKeyDown(GLFW_KEY_A))
            position.x += 0.01f;
        if (app->IsKeyDown(GLFW_KEY_D))
            position.x -= 0.01f;
        if (app->IsKeyDown(GLFW_KEY_SPACE))
            position.y += 0.01f;
        if (app->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
            position.y -= 0.01f;
    }

};