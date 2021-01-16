#pragma once

#include <string>

enum class VirtualKey {
	NONE,
	FORWARD,
	LEFT,
	RIGHT,
	BACK,
	EXIT
};

static std::string virtual_key_to_string(VirtualKey key) {
	switch (key) {
	case VirtualKey::NONE:
		return "NONE";
	case VirtualKey::FORWARD:
		return "FORWARD";
	case VirtualKey::LEFT:
		return "LEFT";
	case VirtualKey::RIGHT:
		return "RIGHT";
	case VirtualKey::BACK:
		return "BACK";
	case VirtualKey::EXIT:
		return "EXIT";
	default: 
		return "INVALID_KEY";
	}
}

class InputSubscriber {

public:
	virtual ~InputSubscriber() {};
	virtual void on_key_press(VirtualKey key) {};
	virtual void on_mouse_move(double dx, double dy) {};
};