#pragma once
#include <render/Ray.hpp>
class Light
{
    glm::vec4 colour;
    glm::vec3 position;
    glm::vec4 rotation;

  public:
    Light()
        : colour({1.0f,1.0f,1.0f,1.0f})
        , position({0.0, 0.0, 0.0})
        , rotation({0.0, 1.0, 0.0, 0.0})
    {
    }

    void setColour(glm::vec4 new_colour)
    {
        colour = new_colour;
    }

    void setPosition(glm::vec3 new_position)
    {
        position = new_position;
    }

    void setRotation(glm::vec4 new_rotation)
    {
        rotation = new_rotation;
    }

    void reset()
    {
        colour = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    }

    const glm::vec4 &getColour() const noexcept
    {
        return colour;
    }
};