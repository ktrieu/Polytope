#include "ShadowMap.h"

#include <resource/ResourceLoader.h>
#include <stdexcept>

ShadowMap::ShadowMap(ResourceLoader& loader) : m_loader(loader) {
	glGenFramebuffers(1, &m_fbo);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	glTexImage3D(
		GL_TEXTURE_2D_ARRAY,
		0,
		GL_DEPTH_COMPONENT,
		SHADOW_MAP_SIZE,
		SHADOW_MAP_SIZE,
		MAX_SHADOW_MAPS,
		0, 
		GL_DEPTH_COMPONENT, 
		GL_FLOAT, 
		NULL
	);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

ShaderProgram& ShadowMap::get_shader() {
	if (m_shader == nullptr) {
		throw std::runtime_error("No shader attached. Call start_shadow_render first.");
	}
	else {
		return *m_shader;
	}
}

void ShadowMap::start_shadow_render() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	m_shader = &m_loader.get_shader("shader/shadow");
	m_shader->use();
}

void ShadowMap::use_shadow_map(int idx) {
	if (idx < 0 || idx >= MAX_SHADOW_MAPS) {
		throw std::runtime_error("Invalid shadow map index.");
	}

	glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_texture, 0, idx);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::finish_shadow_render() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// HACK: we really should fetch the screen size from the App class
	glViewport(0, 0, 720, 480);
	m_shader->unuse();
	m_shader = nullptr;
}