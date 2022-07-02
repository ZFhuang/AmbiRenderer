#pragma once
#include "../core/AppBase.h"
#include "../core/ControlManager.h"
#include "../core/RendererManager.h"
#include "../renderers/Graphics.h"

#include "../renderers/rasterizer/Rasterizer.h"
#include "../renderers/shaders/v_Test.h"

class TestApp : public AppBase
{
public:
	TestApp();

	void GetKeyPress(KeyCode key) noexcept;
	void GetMouseLeftPress(float x, float y) noexcept;

	void Update(float delta_time) noexcept override;

	void Init() noexcept override;

	void Destroy() noexcept override {
		ABR_DEBUG_OUTPUT("TestApp Destroy!");
	}

private:
	void BuildGeometryBuffer();
	void BuildVertexLayout();
	void BuildVertexShader();
	void DrawScene();

private:
	ABR_BYTE_BUFFER pVertexBuffer = nullptr;
	ABR_BYTE_BUFFER pIndexBuffer = nullptr;

private:
	ControlManager* controlManager = nullptr;
	RendererManager* rendererManager = nullptr;
	Rasterizer* renderer = nullptr;
};

struct Vertex {
	VectorF3 position;
	ColorU3 color;
};