#pragma once

#include <string>
#include <vector>

#include <cereal/types/vector.hpp>

struct Texture {
	std::string name;
	int width;
	int height;
	int channels_per_pixel;
	std::vector<unsigned char> data;
	
	template <class Archive>
	void serialize(Archive& archive) {
		archive(name, width, height, channels_per_pixel, data);
	}
};