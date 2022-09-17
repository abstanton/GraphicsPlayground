#include "InputManager.h"

glm::vec2 InputManager::getMouseMovement() {
	double xpos = input_state_.mouse_position[0];
	double ypos = input_state_.mouse_position[1];

	if (first_mouse_movement)
	{
		last_mouse_pos[0] = xpos;
		last_mouse_pos[1] = ypos;
		first_mouse_movement = false;
	}

	double xoffset = xpos - last_mouse_pos[0];
	double yoffset = last_mouse_pos[1] - ypos; // reversed since y-coordinates go from bottom to top

	last_mouse_pos[0] = xpos;
	last_mouse_pos[1] = ypos;

	return { xoffset, yoffset };
}

glm::vec2 InputManager::getScrollOffset() {
	return { input_state_.scroll_offset[0], input_state_.scroll_offset[1] };
}
