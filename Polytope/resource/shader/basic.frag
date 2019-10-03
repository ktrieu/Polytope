#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;

const vec3 light_pos = vec3(4.0, 4.0, -4.0);
const float light_strength = 2;
const vec3 base_color = vec3(0.9, 0.9, 0.9);

void main() {
	//basic lambertian diffuse
	vec3 light_pos_view_space = (view * vec4(light_pos, 1.0)).xyz;
	vec3 to_light = normalize(light_pos_view_space - pos);
	float light_fac = dot(to_light, normal);
	color = vec4(base_color.xyz * light_fac * light_strength, 1.0);
}