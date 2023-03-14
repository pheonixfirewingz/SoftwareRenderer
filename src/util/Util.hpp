#pragma once
#include <cstdint>
#include <mutex>
#ifdef IS_GCC
#    define memory_align(x, v) x __attribute__((aligned(v)))
#endif
#ifdef IS_MSVC
#    define memory_align(x, v) __declspec(align(v)) x
#endif // IS_MSVC