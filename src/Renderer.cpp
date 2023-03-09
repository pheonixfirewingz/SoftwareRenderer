#include "Renderer.h"
#include <cstdlib>
#include <cstring>

#define ARGBToRGBA(a,r,g,b) (r << 24) + (g << 16) + (b << 8) + (a << 0)
#define RGBToRGBA(r,g,b) (r << 24) + (g << 16) + (b << 8) + (255 << 0)
Renderer::~Renderer()
{
    delete camera;
    free(screen_data);
}

Renderer::Renderer(int width, int height) : internal_width(width), internal_height(height)
{
    screen_data = static_cast<uint32_t*>(malloc(width* height * sizeof(uint32_t)));
    camera = new Camera({0,0,0},{1,1,1},{0,0,0,0});
}

void Renderer::clearScreen(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
    __m128i xmm_val = _mm_set1_epi32((ARGBToRGBA(a,r,g,b)));
    for (size_t i = 0; i < internal_width* internal_height; i += 4)
        _mm_store_si128((__m128i*)(screen_data + i), xmm_val);
}

void Renderer::resizeScreen(int width, int height)
{
    screen_data = static_cast<uint32_t*>(realloc(screen_data, (internal_width = width) * (internal_height = height) * sizeof(uint32_t)));
}

const uint32_t *Renderer::getScreenData() const noexcept
{
    return screen_data;
}
