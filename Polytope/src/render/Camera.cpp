#include "Camera.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() : m_pos(0), m_pitch(0), m_yaw(0) {
	recalculate_matrices();
}

Camera::~Camera() {

}

void Camera::update() {
	if (m_dirty) {
		recalculate_matrices();
	}
}

void Camera::translate(glm::vec3& translation) {
	m_pos += translation;
	m_dirty = true;
}

void Camera::rotate(float pitch_delta, float yaw_delta) {
	m_pitch += pitch_delta;
	m_yaw += yaw_delta;
	clamp_angles();
	m_dirty = true;
}

void Camera::set_position(glm::vec3& position) {
	m_pos = glm::vec3(position);
	m_dirty = true;
}

void Camera::set_rotation(float pitch, float yaw) {
	m_pitch = pitch;
	m_yaw = yaw;
	m_dirty = true;
}

void Camera::clamp_angles() {
	m_pitch = std::clamp(m_pitch, PITCH_MIN, PITCH_MAX);
	m_yaw = std::fmodf(m_yaw, 360);
}

void Camera::recalculate_matrices() {
	//setup a default projection matrix, we'll handle screen sizes and such later
	m_proj = glm::perspective(glm::radians(60.0f), 720.0f / 480.0f, 0.01f, 100.0f);
	m_view = glm::mat4(1.0);
	//we have to use inverse transforms for the view matrix
	m_view = glm::translate(m_view, -m_pos);
	m_view = glm::rotate(m_view, m_pitch, glm::vec3(1, 0, 0));
	m_view = glm::rotate(m_view, m_yaw, glm::vec3(0, 1, 0));
	m_dirty = false;
}