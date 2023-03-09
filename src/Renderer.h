#pragma once
#include <cstdint>
#include "math/Vector3.hpp"
#include "math/Matrix4x4.hpp"
#include "render/Camera.hpp"
class Renderer
{
    uint32_t* screen_data;
    uint32_t internal_width;
    uint32_t internal_height;
    Camera* camera;
    public:
    Renderer(int width, int height);
    ~Renderer();
    void clearScreen(uint8_t a,uint8_t r,uint8_t g,uint8_t b);
    void resizeScreen(int width, int height);

    const uint32_t* getScreenData() const noexcept; 

};