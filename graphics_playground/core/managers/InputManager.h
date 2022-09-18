#pragma once
#include <map>
#include <glm/glm.hpp>

enum Key {
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	N_0,
	N_1,
	N_2,
	N_3,
	N_4,
	N_5,
	N_6,
	N_7,
	N_8,
	N_9,
	SPACE,
	ENTER,
	L_SHIFT,
	R_SHIFT,
	L_CTRL,
	R_CTRL,
	TAB,
};

enum KeyState {
	DOWN,
	UP
};

class GraphicsPlayground;

class InputManager
{
	friend GraphicsPlayground;

public:
	static InputManager& get() {
		static InputManager input_manager;
		return input_manager;
	}

	KeyState getKeyState(Key);
	glm::vec2 getMouseMovement();
	glm::vec2 getScrollOffset();

private:
	InputManager() {}

	bool first_mouse_movement = true;
	double last_mouse_pos[2] = {0,0};

	struct InputState {
		double scroll_offset[2];
		double mouse_position[2];
	};

	void mouseMoveCallback(double x_pos, double y_pos) {
		input_state_.mouse_position[0] = x_pos;
		input_state_.mouse_position[1] = y_pos;
	}

	void scrollCallback(double x_offset, double y_offset) {
		input_state_.scroll_offset[0] = x_offset;
		input_state_.scroll_offset[1] = y_offset;
	}

	InputState input_state_{};
};

