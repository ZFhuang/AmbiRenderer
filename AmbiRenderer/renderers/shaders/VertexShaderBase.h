#pragma once
#include<Windows.h>
#include<minwindef.h>
#include"../Graphics.h"

class VertexShaderBase {
public:
    virtual void Run(BYTE* in, BYTE* out, ABR_INPUT_LAYOUT inDesc) = 0;
};
