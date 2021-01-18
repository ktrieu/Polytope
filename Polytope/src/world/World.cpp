#include "World.h"

#include <App.h>
#include <resource/ResourceLoader.h>
#include <random>

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
	// generate a bunch of random entities
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> pos_dist(0.0, 20.0);
	std::uniform_real_distribution<float> rot_dist(0.0, 360.0);
	std::uniform_real_distribution<float> color_dist(0.0, 1.0);
	for (int i = 0; i < 100; i++) {
		m_entities.emplace_back(
			"mesh/wall", 
			"material/test", 
			glm::vec3(pos_dist(rng), pos_dist(rng), pos_dist(rng)), 
			glm::vec3(rot_dist(rng), rot_dist(rng), rot_dist(rng))
		);
	}
	// and a bunch of random lights
	for (int i = 0; i < 20; i++) {
		m_lights.emplace_back(
			glm::vec3(pos_dist(rng), pos_dist(rng), pos_dist(rng)),
			glm::vec3(color_dist(rng), color_dist(rng), color_dist(rng)),
			3
		);
	}
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
	}
}

void World::render(ResourceLoader& loader) {
	for (Entity& entity : m_entities) {
		entity.render(m_renderer);
	}

	for (Light& light : m_lights) {
		m_renderer.draw_light(light);
	}
	m_renderer.render(m_camera, loader);
}