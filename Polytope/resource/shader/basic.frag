#version 330

layout(location = 0) in vec3 pos;

out vec4 color;

const vec3 light_pos = vec3(0, 0, 1.0);
const vec3 base_color = vec3(0.9, 0.9, 0.9);

void main() {
	//this is going to look pretty weird, but we don't have normals yet
	vec3 light_color = (1 / length(light_pos - base_color)) * base_color;
	color = vec4(base_color.xyz, 1.0);
}