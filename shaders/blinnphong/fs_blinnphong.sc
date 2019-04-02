$input v_pos, v_view, v_normal

#include <bgfx_shader.sh>

struct DirLight
{
    vec3 dir;
    vec3 intensity;
};

struct Material
{
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
};

const DirLight main_light = DirLight(vec3(+ 1.0, 0.0, + 2.0), vec3(1.0, 0.9, 0.9));
const DirLight sub_light = DirLight(vec3(- 1.0, 0.0, - 1.0), vec3(0.2, 0.2, 0.5));
const Material material = Material(
    vec3(0.50, 0.50, 0.50),
    vec3(1.00, 1.00, 1.00),
    vec3(0.03, 0.03, 0.03),
    128.0
);
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

vec3 calculateSingleLightShading(DirLight dir_light, Material material, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(dir_light.dir);

    vec3 diffuse = dir_light.intensity * calculateLambertDiffuse(normal, light_dir, material.diffuse);
    vec3 specular = dir_light.intensity * calculateBlinnSpecular(normal, view_dir, light_dir, material.specular, material.shininess);
    vec3 ambient = material.ambient;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 linear_color = vec3(0.0, 0.0, 0.0);

    vec3 normal = normalize(v_normal);
    vec3 view_dir = normalize(- v_view);

    linear_color += calculateSingleLightShading(main_light, material, normal, view_dir);
    linear_color += calculateSingleLightShading(sub_light, material, normal, view_dir);

	vec3 corrected_color = pow(linear_color, vec3_splat(1.0 / gamma));

	gl_FragColor.xyz = corrected_color;
	gl_FragColor.w = 1.0;
}
