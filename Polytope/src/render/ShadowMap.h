#pragma once

#include <glad/glad.h>

class ShaderProgram;
class ResourceLoader;

class ShadowMap {

public:
	ShadowMap(ResourceLoader& loader);
	~ShadowMap() {}

	int get_max_shadow_maps() { return MAX_SHADOW_MAPS; }
	GLuint get_texture() { return m_texture; }
	ShaderProgram& get_shader();

	void start_shadow_render();
	void use_shadow_map(int idx);
	void finish_shadow_render();

private:
	ResourceLoader& m_loader;
	ShaderProgram* m_shader = nullptr;

	const int SHADOW_MAP_SIZE = 2048;
	const int MAX_SHADOW_MAPS = 5;

	GLuint m_fbo = 0;
	GLuint m_texture = 0;

};