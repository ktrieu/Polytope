#pragma once

#include <vector>

#include <world/Entity.h>
#include <world/Light.h>
#include <render/EntityRenderer.h>
#include <render/Camera.h>

class App;
class ResourceLoader;

class World {

public:
	World(App& app);
	~World();

	void load(ResourceLoader& loader);

	void update(double dt);
	void render(ResourceLoader& loader);

private:
	App& m_app;

	std::vector<Entity> m_entities;
	std::vector<Light> m_lights;
	EntityRenderer m_renderer;
	Camera m_camera;
};
