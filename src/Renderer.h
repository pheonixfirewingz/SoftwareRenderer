#pragma once
#include <util/Sse3.h>
#include <render/Camera.hpp>
#include <cstdint>
struct Pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

class Renderer
{
    Pixel* screen_data;
    uint32_t internal_width;
    uint32_t internal_height;
    public:
    Renderer(int width, int height);
    ~Renderer();
    void clearScreen(uint8_t a,uint8_t r,uint8_t g,uint8_t b);
    void resizeScreen(int width, int height);

    void render();

    const Pixel* getScreenData() const noexcept; 

};