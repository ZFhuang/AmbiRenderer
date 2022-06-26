#pragma once
#include "../Graphics.h"

static void v_Test(BYTE* in, BYTE* out, ABR_INPUT_LAYOUT inDesc) noexcept {
    VectorF3* position = reinterpret_cast<VectorF3*>(in + inDesc[0]);
    VectorU3* color = reinterpret_cast<VectorU3*>(in + inDesc[1]);
}