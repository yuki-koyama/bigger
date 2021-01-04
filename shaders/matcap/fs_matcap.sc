$input v_normal

#include <bgfx_shader.sh>

SAMPLER2D(s_tex_matcap, 0);

void main()
{
    vec2 coords = 0.5 * (normalize(v_normal).xy + 1.0);

    gl_FragColor.xyz = texture2D(s_tex_matcap, vec2(coords.x, 1.0 - coords.y)).xyz;
    gl_FragColor.w = 1.0;
}
