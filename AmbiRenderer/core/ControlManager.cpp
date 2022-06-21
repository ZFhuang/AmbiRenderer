#include "ControlManager.h"

ControlManager::ControlManager() noexcept
{
	CleanState();
}

void ControlManager::SetKeyDown(KeyCode k) noexcept
{
	cur_kb_states[unsigned(k)] = KEY_DOWN;
}

void ControlManager::SetKeyUp(KeyCode k) noexcept
{
	cur_kb_states[unsigned(k)] = KEY_UP;
}

void ControlManager::SetMousePos(int x, int y) noexcept
{
	cur_mouse_pos.x = x;
	cur_mouse_pos.y = y;
}

void ControlManager::SetMouseLeft(bool state) noexcept
{
	cur_mouse_left = state;
}

void ControlManager::SetMouseRight(bool state) noexcept
{
	cur_mouse_right = state;
}

void ControlManager::Refresh() noexcept
{
	for (int i = 0; i < MAXKEY; ++i) {
		last_kb_states[i] = cur_kb_states[i];
	}
	last_mouse_pos = cur_mouse_pos;
	last_mouse_left = cur_mouse_left;
	last_mouse_right = cur_mouse_right;
}

void ControlManager::CleanState() noexcept
{
	cur_kb_states = KB_States(MAXKEY, KEY_UP);
	last_kb_states = KB_States(MAXKEY, KEY_UP);
	cur_mouse_left = false;
	last_mouse_left = false;
	cur_mouse_right = false;
	last_mouse_right = false;
	cur_mouse_pos = MousePos();
	last_mouse_pos = MousePos();
}
