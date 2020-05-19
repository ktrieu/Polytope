#include "mat_parse.h"

#include <string>
#include <regex>
#include <stdexcept>
#include <sstream>

struct ParsedLine {
	std::string type;
	std::string name;
	std::string value;
};

// split lines into (type) (name) = (value)
const std::regex LINE_REGEX(R"((?:(\S+) )?(\S+) *= *(.+))");

ParsedLine parse_line(const std::string& line) {
	std::smatch match;
	if (!std::regex_match(line, match, LINE_REGEX)) {
		throw std::runtime_error("Invalid line format.");
	}
	return {
		match[1].str(),
		match[2].str(),
		match[3].str()
	};
}

std::vector<float> split_components(const std::string& value) {
	std::stringstream ss(value);
	std::istream_iterator<float> iter(ss);
	return { iter, std::istream_iterator<float>() };
}

glm::vec2 parse_vec2(const std::string& value) {
	auto components = split_components(value);
	if (components.size() != 2) {
		throw std::runtime_error("Expected 2 vector components, got " + std::to_string(components.size()));
	}
	return glm::vec2(components[0], components[1]);
}

glm::vec3 parse_vec3(const std::string& value) {
	auto components = split_components(value);
	if (components.size() != 3) {
		throw std::runtime_error("Expected 3 vector components, got " + std::to_string(components.size()));
	}
	return glm::vec3(components[0], components[1], components[2]);
}

glm::vec4 parse_vec4(const std::string& value) {
	auto components = split_components(value);
	if (components.size() != 4) {
		throw std::runtime_error("Expected 4 vector components, got " + std::to_string(components.size()));
	}
	return glm::vec4(components[0], components[1], components[2], components[3]);
}

UniformType parse_uniform(const ParsedLine& parsed) {
	if (parsed.type == "float") {
		return UniformType(std::stof(parsed.value));
	}
	else if (parsed.type == "int") {
		return UniformType(std::stoi(parsed.value));
	}
	else if (parsed.type == "string") {
		return UniformType(parsed.value);
	}
	else if (parsed.type == "vec2") {
		return UniformType(parse_vec2(parsed.value));
	}
	else if (parsed.type == "vec3") {
		return UniformType(parse_vec3(parsed.value));
	}
	else if (parsed.type == "vec4") {
		return UniformType(parse_vec4(parsed.value));
	}
	else {
		throw std::runtime_error("Type " + parsed.type + " is not valid.");
	}
}

const std::string TEXTURE_NAME = "texture";
const std::string SHADER_NAME = "shader";

Material parse_material_file(std::ifstream& file) {
	std::string texture_name;
	std::string shader_name;
	std::unordered_map<std::string, UniformType> uniforms;

	std::string line;
	int line_num = 1;
	while (std::getline(file, line)) {
		if (line[0] == '#' || line == "") {
			// it's a comment or empty line
			continue;
		}
		try {
			ParsedLine parsed = parse_line(line);
			if (parsed.name == TEXTURE_NAME) {
				if (parsed.type != "") {
					throw std::runtime_error("Cannot provide type for texture name.");
				}
				texture_name = parsed.value;
			}
			else if (parsed.name == SHADER_NAME) {
				if (parsed.type != "") {
					throw std::runtime_error("Cannot provide type for shader name.");
				}
				shader_name = parsed.value;
			}
			else {
				UniformType uniform_value = parse_uniform(parsed);
				uniforms[parsed.name] = uniform_value;
			}
			line_num++;
		}
		catch (std::runtime_error& e) {
			throw std::runtime_error("Parse error at line " + std::to_string(line_num) + ": " + e.what());
		}
	}

	Material mat;
	mat.texture_name = texture_name;
	mat.shader_name = shader_name;
	mat.uniforms = uniforms;
	return mat;
}