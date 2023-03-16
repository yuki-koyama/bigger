$input v_pos, v_view, v_normal, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_tex_diffuse, 0);

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

uniform vec4 u_params[6];
#define u_specular          u_params[0].xyz
#define u_ambient           u_params[1].xyz
#define u_shininess         u_params[1].w
#define u_dir_light_0_dir   u_params[2].xyz
#define u_dir_light_0_color u_params[3].xyz
#define u_dir_light_1_dir   u_params[4].xyz
#define u_dir_light_1_color u_params[5].xyz

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

vec3 convertToLinear(vec3 gamma_color)
{
    return pow(gamma_color, vec3_splat(gamma));
}

vec3 convertToGamma(vec3 linear_color)
{
    return pow(linear_color, vec3_splat(1.0 / gamma));
}

void main()
{
    vec3 linear_color = vec3(0.0, 0.0, 0.0);

    // Retrieve the diffuse color from sampler
    vec3 diffuse_color = convertToLinear(texture2D(s_tex_diffuse, v_texcoord0).xyz);

    // Assemble the material property
    Material material;
    material.diffuse = diffuse_color;
    material.specular = u_specular;
    material.ambient = u_ambient;
    material.shininess = u_shininess;

    // Note: When the triangle is back-facing, the normal direction will be flipped
    vec3 view_dir = normalize(- v_view);
    vec3 normal = dot(v_normal, view_dir) > 0.0 ? normalize(v_normal) : normalize(- v_normal);

    DirLight dir_light_0, dir_light_1;
    dir_light_0.dir = u_dir_light_0_dir;
    dir_light_0.intensity = u_dir_light_0_color;
    dir_light_1.dir = u_dir_light_1_dir;
    dir_light_1.intensity = u_dir_light_1_color;

    linear_color += calculateSingleLightShading(dir_light_0, material, normal, view_dir);
    linear_color += calculateSingleLightShading(dir_light_1, material, normal, view_dir);

    linear_color += material.ambient;

    vec3 corrected_color = convertToGamma(linear_color);

    gl_FragColor.xyz = corrected_color;
    gl_FragColor.w = 1.0;
}
