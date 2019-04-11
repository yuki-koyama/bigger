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

uniform vec4 u_params[3];
#define u_diffuse   u_params[0].xyz
#define u_dummy_0   u_params[0].w
#define u_specular  u_params[1].xyz
#define u_dummy_1   u_params[1].w
#define u_ambient   u_params[2].xyz
#define u_shininess u_params[2].w

const DirLight main_light = DirLight(vec3(+ 1.0, 0.0, + 2.0), vec3(1.0, 0.9, 0.9));
const DirLight sub_light = DirLight(vec3(- 1.0, 0.0, - 1.0), vec3(0.2, 0.2, 0.5));
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

    return diffuse + specular;
}

void main()
{
    vec3 linear_color = vec3(0.0, 0.0, 0.0);

    Material material = Material(
        u_diffuse,
        u_specular,
        u_ambient,
        u_shininess
    );

    vec3 normal = normalize(v_normal);
    vec3 view_dir = normalize(- v_view);

    linear_color += calculateSingleLightShading(main_light, material, normal, view_dir);
    linear_color += calculateSingleLightShading(sub_light, material, normal, view_dir);

    linear_color += material.ambient;

    vec3 corrected_color = pow(linear_color, vec3_splat(1.0 / gamma));

    gl_FragColor.xyz = corrected_color;
    gl_FragColor.w = 1.0;
}
