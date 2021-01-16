#pragma once

#include <glm/glm.hpp>

class Camera {
public:
	Camera();
	~Camera();

	void update();

	void translate(glm::vec3& translation);
	void rotate(float pitch_delta, float yaw_delta);

	void set_position(glm::vec3& position);
	void set_rotation(float pitch, float yaw);

	void look(glm::vec2& mouse_delta);
	void move(glm::vec3& dir);

	glm::mat4 get_view() { return m_view; }
	glm::mat4 get_proj() { return m_proj; }

private:
	bool m_dirty = false;

	glm::mat4 m_proj;
	glm::mat4 m_view;

	glm::vec3 m_pos;
	float m_pitch;
	float m_yaw;

	const float PITCH_MAX = 85;
	const float PITCH_MIN = -85;

	const float X_SENSITIVITY = 0.001;
	const float Y_SENSITIVITY = 0.001;

	void clamp_angles();
	void recalculate_matrices();
};