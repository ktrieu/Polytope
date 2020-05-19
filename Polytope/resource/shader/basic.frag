#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D tex_sampler;

out vec4 color;

const vec3 light_pos = vec3(4.0, 4.0, -4.0);
const float light_strength = 2;

const float ambient_fac = 0.1f;
const float diffuse_fac = 1.0f;
const float specular_fac = 1.0f;
const vec3 specular_color = vec3(1.0, 1.0, 1.0);
const float shininess = 80.0f;

void main() {
	vec3 n = normalize(normal);
	vec3 to_light = normalize(light_pos - pos);
	float diffuse = max(dot(n, to_light), 0.0);
	
	// phong specular model
	float specular = 0.0f;
	if (diffuse > 0.0) {
		vec3 light_reflected = reflect(-to_light, n);
		float spec_angle = max(dot(light_reflected, normalize(-pos)), 0.0);
		specular = pow(spec_angle, shininess);
	}

	vec3 texture_color = texture(tex_sampler, uv).xyz;

	vec3 final_color = (
		ambient_fac * texture_color +
		diffuse_fac * diffuse * texture_color +
		specular_fac * specular * specular_color
	);

	gl_FragColor = vec4(final_color, 1.0);
}