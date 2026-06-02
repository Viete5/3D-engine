#version 330 core

in vec2 tex_coord;
in vec4 vertex_color;

out vec4 frag_color;

uniform sampler2D font_atlas;
uniform float px_range;

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

float screen_px_range()
{
    vec2 atlas_size = vec2(textureSize(font_atlas, 0));
    vec2 unit_range = vec2(px_range) / atlas_size;
    vec2 screen_tex_size = vec2(1.0) / fwidth(tex_coord);
    return max(0.5, dot(unit_range, screen_tex_size));
}

void main()
{
    vec3 msd = texture(font_atlas, tex_coord).rgb;
    float signed_distance = median(msd.r, msd.g, msd.b);
    float screen_distance = screen_px_range() * (signed_distance - 0.5);
    float alpha = clamp(screen_distance + 0.5, 0.0, 1.0);

    frag_color = vec4(vertex_color.rgb, vertex_color.a * alpha);

    if (frag_color.a <= 0.001) {
        discard;
    }
}
