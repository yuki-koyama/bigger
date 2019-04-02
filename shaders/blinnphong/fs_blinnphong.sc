$input v_pos, v_view, v_normal

#include <bgfx_shader.sh>

const vec3 ambient_color = vec3(0.05, 0.05, 0.05);
const vec3 diffuse_color = vec3(0.20, 0.30, 0.70);
const vec3 specular_color = vec3(1.00, 1.00, 1.00);
const float shininess = 128.0;
const float gamma = 2.2;

vec3 calculateLambertDiffuse(vec3 normal, vec3 light_dir, vec3 diffuse_color)
{
	return max(dot(normal, light_dir), 0.0) * diffuse_color;
}

vec3 calculateBlinnSpecular(vec3 normal, vec3 view_dir, vec3 light_dir, vec3 specular_color, float shininess)
{
	vec3 half_dir = normalize(light_dir + view_dir);
	float angle = max(dot(half_dir, normal), 0.0);
	float strength = pow(angle, shininess);
	return strength * specular_color;
}

void main()
{
	vec3 light_dir = normalize(vec3(1.0, 1.0, 1.0));
	vec3 normal = normalize(v_normal);
	vec3 view_dir = normalize(- v_view);

	vec3 ambient = ambient_color;
	vec3 diffuse = calculateLambertDiffuse(normal, light_dir, diffuse_color);
	vec3 specular = calculateBlinnSpecular(normal, view_dir, light_dir, specular_color, shininess);

	vec3 linear_color = ambient + diffuse + specular;
	vec3 corrected_color = pow(linear_color, vec3_splat(1.0 / gamma));

	gl_FragColor.xyz = corrected_color;
	gl_FragColor.w = 1.0;
}
