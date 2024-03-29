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

#include <vector>
#include <tuple>
#include <memory>
#include <functional>
#include "../Graphics.h"
#include "../../core/RendererBase.h"

class Rasterizer :public RendererBase {
	typedef std::function<void(BYTE* in, BYTE* out, ABR_INPUT_LAYOUT inDesc)> Shader;

public:
	Rasterizer() = default;
	void StartUp() noexcept override;
	void Render() noexcept override;
	~Rasterizer();

public:
	void IASetPrimitiveTopology(const ABR_PRIMITIVE_TOPOLOGY& topology);
	void IASetVertexBuffers(const ABR_BYTE_BUFFER pVertexBuffer, const UINT* pStride, const UINT* pOffset, UINT slot = 0);
	void IASetIndexBuffer(ABR_BYTE_BUFFER pIndexBuffer);
	void IASetInputLayout(ABR_INPUT_LAYOUT pInputLayout, UINT layoutNum);

public:
	void VSSetShader(Shader vertexShader);
	void VSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, const ABR_BYTE_BUFFER pVertexBuffer);
	void VSSetShaderResources(UINT StartSlot, UINT NumBuffers, const ABR_BYTE_BUFFER pVertexBuffer);

public:
	void Draw(UINT vertexCount, UINT startVertexLocation) noexcept;
	void DrawIndexed(UINT indexxCount, UINT startIndexLocation) noexcept;
	void CreateBuffer(const ABR_BUFFER_DESC* pBufferDesc, const void* pInitialData, ABR_BYTE_BUFFER& pOut);

public:
	ABR_BUFFER_DESC mVertexBufferDesc;
	ABR_BUFFER_DESC mIndexBufferDesc;
	Shader mVertexShader;

private:
	void InitStream(UINT count, UINT start);
	void InputAssemblerStage_Indexed(UINT count, UINT start);
	void InputAssemblerStage_Common(UINT count, UINT start);
	void VertexShaderStage();
	void RasterizerStage();
	void PixelShaderStage();
	void OutputMergerStage();

private:
	ABR_PRIMITIVE_TOPOLOGY mPrimitiveTopology;
	ABR_BYTE_BUFFER mpVertexBuffer;
	ABR_BYTE_BUFFER mpIndexBuffer;
	ABR_BYTE_BUFFER mpPositionBuffer;
	ABR_INPUT_LAYOUT mpInputLayout;
	std::vector<BYTE*> mVertexStream;
	std::vector<BYTE*> mPrimitiveStream;

	struct PrimitiveInfo {
		UINT Stride;
		UINT Step;
	} mPrimitiveInfo;
	
	struct VertexInfo {
		UINT Stride;
		UINT Offset;
	} mVertexInfo;
};
