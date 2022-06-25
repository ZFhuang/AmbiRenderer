#pragma once
#include "../Graphics.h"
#include "VertexShaderBase.h"

class TestVertexShader : public VertexShaderBase {
public:
    void Run(BYTE* in, BYTE* out, ABR_INPUT_LAYOUT inDesc) noexcept override final {
        VectorF3* Position = reinterpret_cast<VectorF3*>(in + inDesc[0]);
        VectorF3* Color = reinterpret_cast<VectorF3*>(in + inDesc[1]);
        ABR_DEBUG_OUTPUT(Position->x);
    }
};
