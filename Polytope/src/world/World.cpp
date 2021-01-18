#include "World.h"

#include <App.h>
#include <resource/ResourceLoader.h>
#include <random>

World::World(App& app) : m_app(app) {

}

World::~World() {

}

void World::load(ResourceLoader& loader) {
	loader.load_from_subdirectory("shader/");
	loader.load_from_subdirectory("arena_scene/");
	Mesh& arena = loader.get_mesh("arena_scene/arena/arena");
	Mesh& cube = loader.get_mesh("arena_scene/cube/cube");
	std::vector<Mesh> meshes = { arena, cube };
	m_renderer.upload_meshes(meshes);
	Texture& arena_tex = loader.get_texture("arena_scene/arena/marble");
	Texture& cube_tex = loader.get_texture("arena_scene/cube/brick");
	std::vector<Texture> textures = { arena_tex, cube_tex };
	m_renderer.upload_textures(textures);
	m_entities.emplace_back(
		"arena_scene/arena/arena",
		"arena_scene/arena/arena",
		glm::vec3(0.0f, 0.0f, 0.0f)
	);
	// generate a bunch of random cubes
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> pos_dist(0.0, 10.0);
	std::uniform_real_distribution<float> rot_dist(0.0, 360.0);
	std::uniform_real_distribution<float> color_dist(0.0, 1.0);
	for (int i = 0; i < 5; i++) {
		m_entities.emplace_back(
			"arena_scene/cube/cube", 
			"arena_scene/cube/cube", 
			glm::vec3(pos_dist(rng), 0, pos_dist(rng))
		);
	}
	m_lights.emplace_back(
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		60,
		5
	);
	m_lights.emplace_back(
		glm::vec3(-0.500f, 0.0f, 0.866f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.500f, 0.0f, 0.866f),
		60,
		5
	);
	m_lights.emplace_back(
		glm::vec3(-0.500f, 0.0f, -0.866f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.500f, 0.0f, -0.866f),
		60,
		5
	);
}

void World::update(double dt) {
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
	else if (input.get_key_state(VirtualKey::UP)) {
		m_camera.move({ 0, 1, 0 });
	}
	else if (input.get_key_state(VirtualKey::DOWN)) {
		m_camera.move({ 0, -1, 0 });
	}

	m_camera.update();

	for (Entity& entity : m_entities) {
		entity.update(dt);
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