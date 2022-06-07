#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include "Config.h"
#include "Utils.h"
#include "EngineCore.h"

constexpr unsigned MAXKEY = 255;
constexpr bool KEY_UP = false;
constexpr bool KEY_DOWN = true;

class ControlManager {
public:
	using Key = unsigned char;
	using KB_States = std::vector<bool>;
	using KB_Buffer = std::queue<Key>;

public:
	ControlManager() noexcept;
	void OnKeyDown(Key k) noexcept;
	void OnKeyUp(Key k) noexcept;
	//void SetMouseState() noexcept;
	void Update() noexcept;
	void CleanState() noexcept;

	struct MouseState
	{
		float x = 0;
		float y = 0;
		bool left_pressing = false;
		bool right_pressing = false;
	};

private:
	KB_States cur_kb_states;
	KB_States last_kb_states;
	MouseState cur_mouse;
	MouseState last_mouse;
};
