#version 330

layout(location = 0) in vec3 pos;

uniform mat4 mvp;

layout(location = 0) out vec3 frag_pos;

void main() {
	vec4 transformed_pos = mvp * vec4(pos, 1.0);
	frag_pos = transformed_pos.xyz;
	gl_Position = transformed_pos;
}