#pragma once

#include <unordered_map>
#include <vector>
#include <GLFW/glfw3.h>

enum class VirtualKey {
	NONE,
	FORWARD,
	LEFT,
	RIGHT,
	BACK,
	EXIT
};

static const std::unordered_map<VirtualKey, std::string> VIRTUAL_KEY_STRINGS{
	{ VirtualKey::NONE, "NONE" },
	{ VirtualKey::FORWARD, "FORWARD"},
	{ VirtualKey::LEFT, "LEFT"},
	{ VirtualKey::RIGHT, "RIGHT"},
	{ VirtualKey::BACK, "BACK"},
	{ VirtualKey::EXIT, "EXIT"}
};

static const std::string& virtual_key_to_key(VirtualKey key) {
	return VIRTUAL_KEY_STRINGS.at(key);
}

static const std::unordered_map<int, VirtualKey> GLFW_KEY_VIRTUAL_KEYS{
	{ GLFW_KEY_ESCAPE, VirtualKey::EXIT },
	{ GLFW_KEY_W, VirtualKey::FORWARD },
	{ GLFW_KEY_A, VirtualKey::LEFT },
	{ GLFW_KEY_D, VirtualKey::RIGHT },
	{ GLFW_KEY_S, VirtualKey::BACK },
};

static const VirtualKey glfw_key_to_virtual_key(int key) {
	auto result = GLFW_KEY_VIRTUAL_KEYS.find(key);
	if (result == GLFW_KEY_VIRTUAL_KEYS.end()) {
		return VirtualKey::NONE;
	}
	else {
		return result->second;
	}
}

