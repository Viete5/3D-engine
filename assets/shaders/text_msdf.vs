#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;
layout (location = 3) in vec4 a_color;

out vec2 tex_coord;
out vec4 vertex_color;

uniform mat4 projection;
uniform mat4 model;

void main()
{
    tex_coord = a_tex_coord;
    vertex_color = a_color;
    gl_Position = projection * model * vec4(a_position, 1.0);
}
