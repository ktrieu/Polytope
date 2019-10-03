#pragma once

#include <vector>

#include <world/Entity.h>
#include <render/EntityRenderer.h>
#include <render/Camera.h>

class App;
class ResourceLoader;

class World {

public:
	World(App& app);
	~World();

	void load(ResourceLoader& loader);

	void update();
	void render();

private:
	App& m_app;

	std::vector<Entity> m_entities;
	EntityRenderer m_renderer;
	Camera m_camera;
};
