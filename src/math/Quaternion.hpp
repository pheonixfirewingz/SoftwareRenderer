#pragma once
#include "Sse3.h"
class SSE3Quaternion {
    __m128 q;
public:
    SSE3Quaternion() : q(_mm_setzero_ps()) {}
    SSE3Quaternion(__m128 quat) : q(quat) {}
    SSE3Quaternion(float w, float x, float y, float z) : q(_mm_set_ps(z, y, x, w)) {}

    __m128 data() const {
        return q;
    }

    float w() const {
        return _mm_cvtss_f32(q);
    }

    float x() const {
        __m128 shuf = _mm_shuffle_ps(q, q, _MM_SHUFFLE(1, 0, 0, 0));
        return _mm_cvtss_f32(shuf);
    }

    float y() const {
        __m128 shuf = _mm_shuffle_ps(q, q, _MM_SHUFFLE(2, 2, 1, 1));
        return _mm_cvtss_f32(shuf);
    }

    float z() const {
        __m128 shuf = _mm_shuffle_ps(q, q, _MM_SHUFFLE(3, 3, 3, 2));
        return _mm_cvtss_f32(shuf);
    }

    SSE3Quaternion operator+(const SSE3Quaternion& other) const {
        return SSE3Quaternion(_mm_add_ps(q, other.q));
    }

    SSE3Quaternion operator-(const SSE3Quaternion& other) const {
        return SSE3Quaternion(_mm_sub_ps(q, other.q));
    }
};