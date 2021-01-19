#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) out vec3 pos_view_space;
layout(location = 1) out vec3 normal_view_space;
layout(location = 2) out vec2 frag_uv;
layout(location = 3) out vec3 pos_world_space;

void main() {
	pos_world_space = (model * vec4(pos, 1.0)).xyz;
	pos_view_space = (view * vec4(pos_world_space, 1.0)).xyz;
	normal_view_space = (view * model * vec4(normal, 0.0)).xyz;
	gl_Position = projection * vec4(pos_view_space, 1.0);
	frag_uv = uv;
}