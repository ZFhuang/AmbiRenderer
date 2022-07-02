#pragma once
#include<vector>
#include<unordered_map>
#include<Windows.h>
#include<minwindef.h>

enum class ABR_PRIMITIVE_TOPOLOGY : UINT {
    ABR_PRIMITIVE_TOPOLOGY_UNDEFINED = 0,
    ABR_PRIMITIVE_TOPOLOGY_POINTLIST = 1,
    ABR_PRIMITIVE_TOPOLOGY_LINELIST = 2,
    ABR_PRIMITIVE_TOPOLOGY_TRIANGLETRIP = 3,
    ABR_PRIMITIVE_TOPOLOGY_TRIANGLELIST = 4,
};

using ABR_BYTE_BUFFER = BYTE*;
using ABR_BUFFER_PTR = BYTE*;
using ABR_INPUT_ELEMENT_DESC = UINT;
using ABR_INPUT_LAYOUT = ABR_INPUT_ELEMENT_DESC*;

struct ABR_BUFFER_DESC {
    UINT ByteWidth; // 缓冲区总长度
};

struct VectorF4 {
    FLOAT x;
    FLOAT y;
    FLOAT z;
    FLOAT w;
};

struct VectorF3 {
    FLOAT x;
    FLOAT y;
    FLOAT z;
};

struct VectorU3 {
    UINT x;
    UINT y;
    UINT z;
};

struct ColorU3 {
    UINT r;
    UINT g;
    UINT b;
};

struct ColorF3 {
    FLOAT r;
    FLOAT g;
    FLOAT b;
};