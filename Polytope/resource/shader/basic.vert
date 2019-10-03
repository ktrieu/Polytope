#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) out vec3 pos_view_space;
layout(location = 1) out vec3 normal_view_space;

void main() {
	mat4 mv = view * model;
	pos_view_space = (mv * vec4(pos, 1.0)).xyz;
	normal_view_space = (mv * vec4(normal, 0.0)).xyz;
	gl_Position = projection * vec4(pos_view_space, 1.0);
}