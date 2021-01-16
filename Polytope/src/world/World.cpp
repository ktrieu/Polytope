#include "World.h"

#include <App.h>
#include <resource/ResourceLoader.h>

World::World(App& app) : m_app(app) {

}

World::~World() {

}

void World::load(ResourceLoader& loader) {
	loader.load_from_subdirectory("mesh/");
	loader.load_from_subdirectory("shader/");
	loader.load_from_subdirectory("texture/");
	loader.load_from_subdirectory("material/");
	Mesh& wall = loader.get_mesh("mesh/wall");
	std::vector<Mesh> meshes = { wall };
	m_renderer.upload_meshes(meshes);
	Texture& marble = loader.get_texture("texture/marble");
	std::vector<Texture> textures = { marble };
	m_renderer.upload_textures(textures);
	m_entities.emplace_back("mesh/wall", "material/test", glm::vec3(0.0f, 0.0f, 0.0f));
}

void World::update() {
	auto input = m_app.get_input_manager();
	auto mouse_delta = input.get_mouse_delta();
	m_camera.look(mouse_delta);

	if (input.get_key_state(VirtualKey::FORWARD)) {
		m_camera.move({ 0, 0, -1 });
	}
	else if (input.get_key_state(VirtualKey::BACK)) {
		m_camera.move({ 0, 0, 1 });
	}
	else if (input.get_key_state(VirtualKey::LEFT)) {
		m_camera.move({ -1, 0, 0 });
	}
	else if (input.get_key_state(VirtualKey::RIGHT)) {
		m_camera.move({ 1, 0, 0 });
	}

	m_camera.update();
	for (Entity& entity : m_entities) {
		entity.update();
		m_renderer.draw_entity(entity);
	}
}

void World::render(ResourceLoader& loader) {
	m_renderer.render(m_camera, loader);
}