#include "Renderer.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <render/Ray.hpp>
#include <util/Util.hpp>

Renderer::~Renderer()
{
    free(screen_data);
}

Renderer::Renderer(int width, int height)
    : internal_width(width)
    , internal_height(height)
{
    screen_data = static_cast<Pixel *>(malloc(width * height * sizeof(Pixel)));
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
    screen_data = static_cast<Pixel *>(
        realloc(screen_data, (internal_width = width) * (internal_height = height) * sizeof(Pixel)));
}

#define vec2 Vector2<double>
#define vec3 Vector3<double>
#define vec4 Vector4<double>

void Renderer::render()
{
    vec2 iResolution(internal_width, internal_width);
    for (uint32_t j = 0; j < internal_height; j++)
    {
        if (j > 100 && j < internal_height - 100)
            for (uint32_t i = 0; i < internal_width; i++)
            {
                if (i > 100 && i < internal_width - 100)
                {
                    vec2 uv = vec2(double(i) / double(internal_width), double(j) / double(internal_height));
                    Pixel b = {uint8_t(uv.x() * 255.0), uint8_t(uv.y() * 255.0), 255, 255};
                    screen_data[j * internal_width + i] = b;
                }
            }
    }
}

const Pixel *Renderer::getScreenData() const noexcept
{
    return screen_data;
}
