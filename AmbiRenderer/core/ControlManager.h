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
	void SetMousePos(int x, int y) noexcept;
	void SetMouseLeft(bool state) noexcept;
	void SetMouseRight(bool state) noexcept;
	template<typename T>
	void Update(T* callback) noexcept;
	void Refresh() noexcept;
	void CleanState() noexcept;

	struct MousePos
	{
		int x = 0;
		int y = 0;

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
GEN_STATIC_DYN_CALL_1(GetKeyHold, KeyCode _1, ControlManager);
GEN_STATIC_DYN_CALL_2(GetMouseMove, int _1, int _2, ControlManager);
GEN_STATIC_DYN_CALL_2(GetMouseLeftHold, int _1, int _2, ControlManager);
GEN_STATIC_DYN_CALL_2(GetMouseLeftDown, int _1, int _2, ControlManager);
GEN_STATIC_DYN_CALL_2(GetMouseLeftUp, int _1, int _2, ControlManager);
GEN_STATIC_DYN_CALL_2(GetMouseRightHold, int _1, int _2, ControlManager);
GEN_STATIC_DYN_CALL_2(GetMouseRightDown, int _1, int _2, ControlManager);
GEN_STATIC_DYN_CALL_2(GetMouseRightUp, int _1, int _2, ControlManager);

template<typename T>
void ControlManager::Update(T* callback) noexcept{
	// update keyboard events
	for (int i = 0; i < MAXKEY; ++i) {
		if (last_kb_states[i] == KEY_DOWN && cur_kb_states[i] == KEY_DOWN) {
			// GetKeyHold
			_ControlManager::GetKeyHold<T>(callback, i);
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
		// GetMouseLeftHold
		_ControlManager::GetMouseLeftHold<T>(callback, cur_mouse_pos.x, cur_mouse_pos.y);
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
		// GetMouseRightHold
		_ControlManager::GetMouseRightHold<T>(callback, cur_mouse_pos.x, cur_mouse_pos.y);
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