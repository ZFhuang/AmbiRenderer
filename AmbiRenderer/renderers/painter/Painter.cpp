#include "Painter.h"

void Painter::StartUp() noexcept
{
	RendererBase::StartUp();
	layers = std::vector<Layer>(layer_num);
}

void Painter::Render() noexcept
{
	// do nothing now
	for (int layer_idx = 0; layer_idx < layer_num; ++layer_idx) {
		if (layers[layer_idx].is_using) {
			DrawLayer(layer_idx);
		}
	}
	Sleep(10);
}

void Painter::SetLayer(p_frame_buffer inp_layer, int layer_idx, float alpha) noexcept
{
	if (layers[layer_idx].is_using == false) {
		layers[layer_idx].is_using = true;
	}
	layers[layer_idx].p_frame = inp_layer;
	layers[layer_idx].alpha = alpha;
}

void Painter::DrawLayer(int layer_idx) noexcept {
	static int sum_elem = frame_height * frame_width;
	float alpha = layers[layer_idx].alpha;
	float minus_alpha = 1 - alpha;
	p_frame_buffer layer = layers[layer_idx].p_frame;
	for (int idx = 0; idx < sum_elem; ++idx) {
		p_back_buffer[idx] = p_back_buffer[idx]* alpha + layer[idx]*minus_alpha;
	}
}

Painter::~Painter()
{
	RendererBase::~RendererBase();
}
