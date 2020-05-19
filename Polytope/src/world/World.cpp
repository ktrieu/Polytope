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
	ShaderProgram& basic_shader = loader.get_shader("shader/basic");
	m_renderer.load_shader(basic_shader);
	Mesh& suzanne = loader.get_mesh("mesh/suzanne");
	std::vector<Mesh> meshes = { suzanne };
	m_renderer.upload_meshes(meshes);
	m_entities.emplace_back(suzanne, glm::vec3(0.0f, 0.0f, -3.0f));
}

void World::update() {
	m_camera.update();
	for (Entity& entity : m_entities) {
		entity.update();
		m_renderer.draw_entity(entity);
	}
}

void World::render() {
	m_renderer.render(m_camera);
}