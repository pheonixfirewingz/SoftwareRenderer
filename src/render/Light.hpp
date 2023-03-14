#pragma once
#include <render/Ray.hpp>
class Light
{
    Colour colour;
    Vec3 position;
    Vec4 rotation;

  public:
    Light()
        : colour({1.0})
        , position({0.0, 0.0, 0.0})
        , rotation({0.0, 1.0, 0.0})
    {
    }

    void setColour(Colour new_colour)
    {
        colour = new_colour;
    }

    void setPosition(Vec3 new_position)
    {
        position = new_position;
    }

    void setRotation(Vec4 new_rotation)
    {
        rotation = new_rotation;
    }

    void reset()
    {
        colour = Colour(1.0);
    }

    const Colour &getColour() const noexcept
    {
        return colour;
    }
};