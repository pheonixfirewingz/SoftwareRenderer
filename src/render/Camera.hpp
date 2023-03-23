#pragma once
#include <App.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <render/Ray.hpp>
class Camera
{
    // temp
    friend App;
    //----
    glm::vec3 position;
    glm::vec3 rotation;

  public:
    Camera()
        : position(0, 0, 0)
        , rotation(0, 0, 0)
    {
    }

    void update(App *app)
    {
        if (app->IsKeyDown(GLFW_KEY_W))
            position.z += 0.1f;
        if (app->IsKeyDown(GLFW_KEY_S))
            position.z -= 0.1f;
        if (app->IsKeyDown(GLFW_KEY_D))
            position.x += 0.1f;
        if (app->IsKeyDown(GLFW_KEY_A))
            position.x -= 0.1f;
        if (app->IsKeyDown(GLFW_KEY_SPACE))
            position.y += 0.1f;
        if (app->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
            position.y -= 0.1f;
    }

    glm::vec3 getRot() const noexcept
    {
        return rotation;
    }

    glm::vec3 getPos() const noexcept
    {
        return position;
    }
};