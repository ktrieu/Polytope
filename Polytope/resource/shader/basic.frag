#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct Light {
	vec3 pos;
	vec3 color;
	float strength;
};

uniform int num_lights = 0;
uniform Light lights[20];

uniform sampler2D tex_sampler;

out vec4 color;

const float ambient_fac = 0.5f;
const float specular_fac = 1.0f;
const vec3 specular_color = vec3(1.0, 1.0, 1.0);
const float shininess = 80.0f;

void main() {
	vec3 final_color;
	vec3 n = normalize(normal);
	vec3 texture_color = texture(tex_sampler, uv).xyz;

	for (int i = 0; i < num_lights; i++) {
		Light light = lights[i];

		float attenuation = 1 / distance(light.pos, pos);
		vec3 to_light = normalize(light.pos - pos);
		float diffuse = max(dot(n, to_light), 0.0);
	
		float specular = 0.0f;
		if (diffuse > 0.0) {
			vec3 light_reflected = reflect(-to_light, n);
			float spec_angle = max(dot(light_reflected, normalize(-pos)), 0.0);
			specular = pow(spec_angle, shininess);
		}

		vec3 light_color = light.strength * light.color * attenuation;

		vec3 ambient_color = ambient_fac * light_color * texture_color;
		vec3 diffuse_color = diffuse * light_color * texture_color;
		vec3 specular_color = attenuation * specular_fac * specular * specular_color;

		final_color += vec3(
			ambient_color + diffuse_color + specular_color
		);
	}

	gl_FragColor = vec4(final_color, 1.0);
}