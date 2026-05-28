#version 330 core

struct Light {
    vec3 position;
    vec3 color;
};

struct Material {
    vec4 base_color;
    int has_base_color_texture;
    sampler2D base_color_texture;
};

in vec3 frag_position;
in vec3 normal;
in vec2 tex_coord;

out vec4 frag_color;

uniform int light_count;
uniform Light lights[16];
uniform Material material;

void main()
{
    vec4 surface_color = material.base_color;

    if (material.has_base_color_texture != 0) {
        surface_color *= texture(material.base_color_texture, tex_coord);
    }

    vec3 normalized_normal = normalize(normal);
    vec3 lighting = vec3(0.2);

    for (int i = 0; i < light_count; ++i) {
        vec3 light_direction = normalize(lights[i].position - frag_position);
        float diffuse_strength = max(dot(normalized_normal, light_direction), 0.0);
        lighting += diffuse_strength * lights[i].color;
    }

    frag_color = vec4(surface_color.rgb * lighting, surface_color.a);
}