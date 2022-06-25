#pragma once
#include <string>
#include <memory>
#include "../../core/RendererBase.h"

class Painter :public RendererBase {
public:
	void StartUp() noexcept override;
	void Render() noexcept override;

public:
	void SetLayer(p_frame_buffer inp_layer, int layer_idx, float alpha = 0.0f) noexcept;

public:
	Painter() = default;
	void DrawLayer(int layer_idx) noexcept;
	~Painter();

private:
	struct Layer {
		p_frame_buffer p_frame = nullptr;
		bool is_using = false;
		float alpha = 0.0f;
	};

private:
	const int layer_num = 10;
	std::vector<Layer> layers;
};