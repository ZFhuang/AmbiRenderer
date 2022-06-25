#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <type_traits>
#include "Config.h"
#include "Utils.h"

constexpr unsigned MAXKEY = 255;
constexpr bool KEY_UP = false;
constexpr bool KEY_DOWN = true;

class ControlManager {
public:
	using KB_States = std::vector<bool>;
	using KB_Buffer = std::queue<KeyCode>;
	using Mouse_Press = bool;

public:
	ControlManager() noexcept;
	void SetKeyDown(KeyCode k) noexcept;
	void SetKeyUp(KeyCode k) noexcept;
	void SetMousePos(float x, float y) noexcept;
	void SetMouseLeft(bool state) noexcept;
	void SetMouseRight(bool state) noexcept;
	template<typename T>
	void Update(T* callback) noexcept;
	void Refresh() noexcept;
	void CleanState() noexcept;

	struct MousePos
	{
		float x = 0;
		float y = 0;

		bool operator!=(const MousePos& other) const noexcept {
			return x != other.x || y != other.y;
		}
	};

private:
	KB_States cur_kb_states;
	KB_States last_kb_states;
	MousePos cur_mouse_pos;
	MousePos last_mouse_pos;
	Mouse_Press cur_mouse_left;
	Mouse_Press last_mouse_left;
	Mouse_Press cur_mouse_right;
	Mouse_Press last_mouse_right;

};

GEN_STATIC_DYN_CALL_1(GetKeyUp, KeyCode _1, ControlManager);
GEN_STATIC_DYN_CALL_1(GetKeyDown, KeyCode _1, ControlManager);
GEN_STATIC_DYN_CALL_1(GetKeyPress, KeyCode _1, ControlManager);
GEN_STATIC_DYN_CALL_2(GetMouseMove, float _1, float _2, ControlManager);
GEN_STATIC_DYN_CALL_2(GetMouseLeftPress, float _1, float _2, ControlManager);
GEN_STATIC_DYN_CALL_2(GetMouseLeftDown, float _1, float _2, ControlManager);
GEN_STATIC_DYN_CALL_2(GetMouseLeftUp, float _1, float _2, ControlManager);
GEN_STATIC_DYN_CALL_2(GetMouseRightPress, float _1, float _2, ControlManager);
GEN_STATIC_DYN_CALL_2(GetMouseRightDown, float _1, float _2, ControlManager);
GEN_STATIC_DYN_CALL_2(GetMouseRightUp, float _1, float _2, ControlManager);

template<typename T>
void ControlManager::Update(T* callback) noexcept{
	// update keyboard events
	for (int i = 0; i < MAXKEY; ++i) {
		if (last_kb_states[i] == KEY_DOWN && cur_kb_states[i] == KEY_DOWN) {
			// GetKeyPress
			_ControlManager::GetKeyPress<T>(callback, i);
		}
		else if (last_kb_states[i] == KEY_UP && cur_kb_states[i] == KEY_DOWN) {
			// GetKeyDown
			_ControlManager::GetKeyDown<T>(callback, i);
		}
		else if (last_kb_states[i] == KEY_DOWN && cur_kb_states[i] == KEY_UP) {
			// GetKeyUp
			_ControlManager::GetKeyUp<T>(callback, i);
		}
	}

	// update mouse events
	if (cur_mouse_pos != last_mouse_pos) {
		// GetMouseMove
		_ControlManager::GetMouseMove<T>(callback, cur_mouse_pos.x, cur_mouse_pos.y);
	}
	if (cur_mouse_left == true && last_mouse_left == true) {
		// GetMouseLeftPress
		_ControlManager::GetMouseLeftPress<T>(callback, cur_mouse_pos.x, cur_mouse_pos.y);
	}
	else if (cur_mouse_left == true && last_mouse_left == false) {
		// GetMouseLeftDown
		_ControlManager::GetMouseLeftDown<T>(callback, cur_mouse_pos.x, cur_mouse_pos.y);
	}
	else if (cur_mouse_left == false && last_mouse_left == true) {
		// GetMouseLeftUp
		_ControlManager::GetMouseLeftUp<T>(callback, cur_mouse_pos.x, cur_mouse_pos.y);
	}
	if (cur_mouse_right == true && last_mouse_right == true) {
		// GetMouseRightPress
		_ControlManager::GetMouseRightPress<T>(callback, cur_mouse_pos.x, cur_mouse_pos.y);
	}
	else if (cur_mouse_right == true && last_mouse_right == false) {
		// GetMouseRightDown
		_ControlManager::GetMouseRightDown<T>(callback, cur_mouse_pos.x, cur_mouse_pos.y);
	}
	else if (cur_mouse_right == false && last_mouse_right == true) {
		// GetMouseRightUp
		_ControlManager::GetMouseRightUp<T>(callback, cur_mouse_pos.x, cur_mouse_pos.y);
	}
	Refresh();
}