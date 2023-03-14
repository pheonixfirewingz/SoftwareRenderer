#pragma once
#include <cstdint>
#include <render/Camera.hpp>
#include <render/Light.hpp>
#include <util/Sse3.h>

#define REFRACTAL_MAX_LIGHTS 100

class Renderer
{
    uint32_t internal_width;
    uint32_t internal_height;
    struct Pixel
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    } *screen_data;
    struct Lights
    {
        Light light;
        bool is_free = true;
    } lights[REFRACTAL_MAX_LIGHTS];

  protected:
    void processPixel(const uint64_t x, const uint64_t y);

  public:
    Renderer(int width, int height);
    ~Renderer();
    void clearScreen(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
    void resizeScreen(int width, int height);

    uint64_t genLight(Vec4 colour);
    void setLightPosition(const uint64_t id,Vec3 position);
    void destroyLight(uint64_t id);

    void render();

    const void *getScreenData() const noexcept;
};