#pragma once
//#include <utility>
//#include <cstdlib>
//#include <string>
//#include <limits>
//#include <cmath>
//
//#include "tgaimage.h"
//#include "model.h"
//#include "gl_soft.h"
//#include "../shaders/v_Basic.cpp"
//#include "../shaders/f_Phong.cpp"
//#include "../shaders/f_Gouraud.cpp"
//#include "../shaders/f_Stylized.cpp"
//#include "../shaders/f_Depth.cpp"
//#include "../shaders/f_Normal.cpp"
//#include "../shaders/f_AO.cpp"
//#include "../shaders/f_SSAO.cpp"

#include <string>
#include <memory>
#include <functional>
#include "../Graphics.h"
#include "../../core/RendererBase.h"
#include "../shaders/VertexShaderBase.h"

class Rasterizer :public RendererBase {
public:
	Rasterizer() = default;
	void StartUp() noexcept override;
	void Render() noexcept override;
	~Rasterizer();

public:
	void IASetPrimitiveTopology(const ABR_PRIMITIVE_TOPOLOGY& topology);
	void IASetVertexBuffers(ABR_BYTE_BUFFER pVertexBuffer);
	void IASetIndexBuffers(ABR_BYTE_BUFFER pIndexBuffer);
	void IASetInputLayout(ABR_INPUT_LAYOUT pInputLayout, UINT layoutNum);

public:
	void VSSetShader(VertexShaderBase* pVertexShader);

public:
	void CreateBuffer(const ABR_BUFFER_DESC* pBufferDesc, const void* pInitialData, ABR_BYTE_BUFFER pOut);
	ABR_BUFFER_DESC mVertexBufferDesc;
	ABR_BUFFER_DESC mIndexBufferDesc;
	VertexShaderBase* mpVertexShader;

public:
	void Draw(UINT vertexCount, UINT startVertexLocation) noexcept;
	void DrawIndexed(UINT indexxCount, UINT startIndexLocation) noexcept;

private:
	void InputAssemblerStage();
	void VertexShaderStage();
	void RasterizerStage();
	void PixelShaderStage();
	void OutputMergerStage();

private:
	ABR_PRIMITIVE_TOPOLOGY mPrimitiveTopology;
	ABR_BYTE_BUFFER mpVertexBuffer;
	ABR_BYTE_BUFFER mpIndexBuffer;
	ABR_INPUT_LAYOUT mpInputLayout;
};
