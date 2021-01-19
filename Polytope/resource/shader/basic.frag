#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 world_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct Light {
	vec3 pos;
	vec3 color;
	vec3 dir;
	float fov;
	float strength;
	mat4 view;
	mat4 proj;
};

uniform int num_lights = 0;
uniform Light lights[20];

uniform sampler2D tex_sampler;
uniform sampler2DArray shadow_maps;

out vec4 color;

const float ambient_fac = 0.01f;
const vec3 specular_color = vec3(1.0, 1.0, 1.0);
const float shininess = 80.0f;

float get_diffuse(vec3 n, vec3 to_light) {
	return max(dot(n, to_light), 0.0);
}

float get_specular(vec3 n, vec3 to_light, float shininess) {
	vec3 light_reflected = reflect(-to_light, n);
	float spec_angle = max(dot(light_reflected, normalize(-pos)), 0.0);
	return pow(spec_angle, shininess);
}

float is_in_light_dir(vec3 to_light, vec3 light_dir, float light_fov) {
	float angle = degrees(acos(dot(-to_light, light_dir)));
	float fov_lower = light_fov * 0.9;
	return 1 - smoothstep(fov_lower, light_fov, angle);
}

vec3 get_light_space_pos(vec3 world_pos, int light_idx) {
	Light light = lights[light_idx];
	vec4 pos_homogenous = light.proj * light.view * vec4(world_pos, 1.0);
	return (pos_homogenous.xyz / pos_homogenous.w) * 0.5 + 0.5;
}

const float SHADOW_BIAS_MAX = 0.01;
const float SHADOW_BIAS_MIN = 0.005;

float get_shadow(vec3 light_space_pos, vec3 n, vec3 to_light, int light_idx) {
	float shadow_map_depth = texture(shadow_maps, vec3(light_space_pos.xy, light_idx)).x;
	float light_depth = light_space_pos.z;

	float bias = max(SHADOW_BIAS_MAX * (1.0 - dot(n, to_light)), SHADOW_BIAS_MIN);  
	float shadow = (light_depth) > shadow_map_depth ? 1.0 : 0.0;

	return 1.0f - shadow;
}

void main() {
	vec3 final_color;
	vec3 n = normalize(normal);
	vec3 texture_color = texture(tex_sampler, uv).xyz;

	for (int i = 0; i < num_lights; i++) {
		Light light = lights[i];

		float attenuation = 1 / distance(light.pos, pos);
		vec3 to_light = normalize(light.pos - pos);

		float diffuse = get_diffuse(n, to_light);
		float specular = get_specular(n, to_light, shininess);

		vec3 light_space_pos = get_light_space_pos(world_pos, i);
		float shadow = get_shadow(light_space_pos, n, to_light, i);

		float is_in_light_dir = is_in_light_dir(to_light, light.dir, light.fov);

		vec3 light_color = light.strength * light.color * attenuation;

		vec3 ambient_color = ambient_fac * texture_color;
		vec3 diffuse_color = is_in_light_dir * shadow * diffuse * light_color * texture_color;
		vec3 specular_color = is_in_light_dir * shadow * specular * light_color * specular_color * 0;

		final_color += vec3(
			ambient_color + diffuse_color + specular_color
		);
	}

	gl_FragColor = vec4(final_color, 1.0);
}