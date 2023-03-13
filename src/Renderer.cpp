#include "Renderer.h"
#include <cstdlib>
#include <cstring>
#include <render/Ray.hpp>
#include <chrono>
#include <ctime>  

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
    uint32_t colour = (uint32_t)(r << 24) + (uint32_t)(g << 16) + (uint32_t)(b << 8) + (uint32_t)(a << 0);
    __m128i xmm_val = _mm_set1_epi32(colour);
    for (size_t i = 0; i < internal_width * internal_height; i += 4)
        _mm_store_si128((__m128i *)(screen_data + i), xmm_val);
}

void Renderer::resizeScreen(int width, int height)
{
    screen_data = static_cast<uint32_t *>(
        realloc(screen_data, (internal_width = width) * (internal_height = height) * sizeof(uint32_t)));
}

static int fix(double value)
{
    return (unsigned char)((value - 0) * (255 / (255 - 0)));
}
//TEMP ------
#define vec2 Vector2<double>
#define vec3 Vector3<double>
#define vec4 Vector4<double>

#define MAX_STEPS 100
#define MAX_DIST 100.
#define SURF_DIST .01

double GetDist(vec3 p)
{
    vec4 s = vec4(0, 1, 6, 1);

    double sphereDist = (p - vec3(s.x(), s.y(), s.z())).length() - s.w();
    double planeDist = p.y();

    double d = std::min(sphereDist, planeDist);
    return d;
}

double RayMarch(vec3 ro, vec3 rd)
{
    double dO = 0.;

    for (int i = 0; i < MAX_STEPS; i++)
    {
        vec3 p = ro + rd * dO;
        double dS = GetDist(p);
        dO += dS;
        if (dO > MAX_DIST || dS < SURF_DIST)
            break;
    }

    return dO;
}

vec3 GetNormal(vec3 p)
{
    double d = GetDist(p);
    vec2 e = vec2(.01, 0);
    vec3 n = d - vec3(GetDist(p - vec3(e.x(), e.y(), e.y())), GetDist(p - vec3(e.y(), e.x(), e.y())),
                      GetDist(p - vec3(e.y(), e.y(), e.x())));

    return unit_vector(n);
}

double GetLight(vec3 p)
{
    vec3 lightPos = vec3(0, 5, 6);
    vec2 to_add = vec2(sin(100), cos(100)) * 2.;
    lightPos.addX(to_add.x());
    lightPos.addZ(to_add.y());
    vec3 l = unit_vector(lightPos - p);
    vec3 n = GetNormal(p);

    double dif = std::clamp(dot(n, l), 0., 1.);
    double d = RayMarch(p + n * SURF_DIST * 2., l);
    if (d < vec3(lightPos - p).length())
        dif *= .1;

    return dif;
}
// ------

void Renderer::render()
{
    vec2 iResolution(internal_width, internal_width);
    for (int32_t j = internal_height - 1; j >= 0; --j)
    {
        for (uint32_t i = 0; i < internal_width; ++i)
        {
            vec2 uv = (vec2(i,j) - .5 * iResolution) / iResolution.y();

            vec3 col = vec3(0);

            vec3 ro = vec3(0, 1, 0);
            vec3 rd = unit_vector(vec3(uv.x(), uv.y(), 1));

            double d = RayMarch(ro, rd);

            vec3 p = ro + rd * d;

            double dif = GetLight(p);
            col = vec3(dif);

            col = pow(col,vec3(.4545)); // gamma correction


            screen_data[i + j] = ((fix(col.x()) >> 24) + (fix(col.y()) >> 16) + (fix(col.z()) >> 8) + fix(col.x()));
        }
    }
}

const uint32_t *Renderer::getScreenData() const noexcept
{
    return screen_data;
}
