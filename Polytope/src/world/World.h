#pragma once

#include <vector>

#include <world/Entity.h>
#include <render/EntityRenderer.h>
#include <render/Camera.h>
#include <input/InputSubscriber.h>

class App;
class ResourceLoader;

class World : public InputSubscriber {

public:
	World(App& app);
	~World();

	void load(ResourceLoader& loader);

	void update();
	void render(ResourceLoader& loader);

	void on_key_press(VirtualKey key) override;
	void on_mouse_move(double x, double y) override;

private:
	App& m_app;

	std::vector<Entity> m_entities;
	EntityRenderer m_renderer;
	Camera m_camera;
};
