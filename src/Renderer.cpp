#include "Renderer.h"
#include "util/Sse3.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

Renderer::~Renderer()
{
    free(screen_data);
}

Renderer::Renderer(int width, int height)
    : internal_width(width)
    , internal_height(height)
{
    screen_data = static_cast<uint32_t *>(malloc(width * height * sizeof(uint32_t)));
}

void Renderer::clearScreen(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
    __m128i xmm_val = _mm_set1_epi32((r << 24) + (g << 16) + (b << 8) + (a << 0));
    for (size_t i = 0; i < internal_width * internal_height; i += 4)
        _mm_store_si128((__m128i *)(screen_data + i), xmm_val);
}

void Renderer::resizeScreen(int width, int height)
{
    screen_data = static_cast<uint32_t *>(
        realloc(screen_data, (internal_width = width) * (internal_height = height) * sizeof(uint32_t)));
}

void Renderer::render()
{
    for (int32_t j = internal_height - 1; j >= 0; --j)
    {
        for (uint32_t i = 0; i < internal_width; ++i)
        {
            screen_data[i + j] = (55 << 24) + (55 << 16) + (55 << 8) + (255 << 0);
        }
        std::cout << '\n';
    }
}

const uint32_t *Renderer::getScreenData() const noexcept
{
    return screen_data;
}
