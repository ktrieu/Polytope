#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 mvp;

layout(location = 0) out vec3 frag_pos;
layout(location = 1) out vec3 frag_normal;

void main() {
	vec4 transformed_pos = mvp * vec4(pos, 1.0);
	frag_pos = transformed_pos.xyz;
	frag_normal = normal;
	gl_Position = transformed_pos;
}