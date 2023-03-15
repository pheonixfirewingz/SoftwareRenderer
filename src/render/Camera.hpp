#pragma once
#include <App.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <render/Ray.hpp>
class Camera
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::mat4x4 cam_matrix;
    glm::mat4x4 cam_projection;
  public:
    Camera()
        : position(0, 0, 0)
        , rotation(0, 1, 0)
    {
    }

    void updateView(int width,int height)
    {
        cam_projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
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
        cam_matrix = glm::rotate(glm::translate(glm::mat4x4(1.0f), position),glm::radians(45.0f),rotation);
        cam_matrix = cam_projection * cam_matrix;
    }

    glm::mat4x4 getCameraMatrix() const noexcept
    {
        return cam_matrix;
    }
};