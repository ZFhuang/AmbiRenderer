#include "ControlManager.h"

ControlManager::ControlManager() noexcept
{
	CleanState();
}

void ControlManager::OnKeyDown(Key k) noexcept
{
	cur_kb_states[unsigned(k)] = KEY_DOWN;
}

void ControlManager::OnKeyUp(Key k) noexcept
{
	cur_kb_states[unsigned(k)] = KEY_UP;
}

void ControlManager::Update() noexcept
{
	// call keyboard events
	for (int i = 0; i < MAXKEY; ++i) {
		if (last_kb_states[i] == KEY_DOWN && cur_kb_states[i] == KEY_DOWN) {
		}
		else if (last_kb_states[i] == KEY_UP && cur_kb_states[i] == KEY_DOWN) {
			last_kb_states[i] = KEY_DOWN;
			Singleton<EngineCore>::GetInstance()->ReactFunc(i);
		}
		else if (last_kb_states[i] == KEY_DOWN && cur_kb_states[i] == KEY_UP) {
			last_kb_states[i] = KEY_UP;
		}
	}

	// call mouse events
	// ...
}

void ControlManager::CleanState() noexcept
{
	cur_kb_states = KB_States(MAXKEY, KEY_UP);
	last_kb_states = KB_States(MAXKEY, KEY_UP);
	cur_mouse = MouseState();
	last_mouse = MouseState();
}
