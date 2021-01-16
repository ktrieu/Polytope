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
	Texture& taco = loader.get_texture("texture/taco");
	std::vector<Texture> textures = { taco };
	m_renderer.upload_textures(textures);
	m_entities.emplace_back("mesh/wall", "material/test", glm::vec3(0.0f, 0.0f, -3.0f));
}

void World::update() {
	m_camera.update();
	for (Entity& entity : m_entities) {
		entity.update();
		m_renderer.draw_entity(entity);
	}
}

void World::render(ResourceLoader& loader) {
	m_renderer.render(m_camera, loader);
}

void World::on_key_press(VirtualKey key) {
	std::cout << "KEY PRESSED: " << virtual_key_to_string(key) << "\n";
}

void World::on_mouse_move(double dx, double dy) {
	std::cout << "MOUSE MOVED: (" << dx << ", " << dy << ")\n";
}