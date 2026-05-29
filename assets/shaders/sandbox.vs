#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;
layout (location = 3) in vec4 a_color;

out vec3 frag_position;
out vec3 normal;
out vec2 tex_coord;
out vec4 vertex_color;

uniform mat4 model;
uniform mat4 normal_matrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 world_position = model * vec4(a_position, 1.0);

    frag_position = world_position.xyz;
    normal = normalize(mat3(normal_matrix) * a_normal);
    tex_coord = a_tex_coord;
    vertex_color = a_color;

    gl_Position = projection * view * world_position;
}
