#include "Renderer.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _OPENMP
#    include <omp.h>
#endif
#include <render/Ray.hpp>
#include <util/Util.hpp>

Renderer::~Renderer()
{
    delete screen_data;
}

Renderer::Renderer(int width, int height)
    : internal_width(width)
    , internal_height(height)
{
    screen_data = new Pixel[width * height];
}

void Renderer::clearScreen(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
    Pixel pixel = Pixel({r, g, b, a});
    __m128i pixel_data = _mm_set_epi32(r, g, b, a);
    uint32_t pixel_count = internal_width * internal_height;
    uint32_t i = 0;
    for (; i + 3 < pixel_count; i += 4)
        _mm_storeu_si128((__m128i *)&screen_data[i], pixel_data);

    for (; i < pixel_count; i++)
        screen_data[i] = pixel;
}

void Renderer::resizeScreen(int width, int height)
{
    screen_data =
        static_cast<Pixel *>(realloc(screen_data, static_cast<size_t>(internal_width = width) *
                                                      static_cast<size_t>(internal_height = height) * sizeof(Pixel)));
}

uint64_t Renderer::genLight(Vec4 colour)
{
    for (uint64_t i = 0; i < (REFRACTAL_MAX_LIGHTS - 1); i++)
    {
        if (lights[i].is_free)
        {
            lights[i].light.setColour(colour);
            return i;
        }
    }
    return REFRACTAL_MAX_LIGHTS;
}

void Renderer::setLightPosition(const uint64_t id, Vec3 position)
{
    lights[id].light.setPosition(position);
}

void Renderer::destroyLight(uint64_t id)
{
    lights[id].is_free = true;
    lights[id].light.reset();
}

void Renderer::processPixel(const uint64_t x, const uint64_t y)
{
    Pixel b = {uint8_t((double(x) / internal_width) * 255.0),
                     uint8_t((double(y) / internal_height) * 255.0), 255, 255};
    screen_data[y * internal_width + x] = b;
}

void Renderer::render()
{
#ifdef _OPENMP
#    pragma omp parallel for
#endif
    for (int64_t j = 0; j < internal_height; j++)
        for (int64_t i = 0; i < internal_width; i++)
            processPixel(i, j);
}

const void *Renderer::getScreenData() const noexcept
{
    return (const void*)screen_data;
}
