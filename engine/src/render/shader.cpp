#include "../include/render/shader.hpp"

namespace engine::render {

Shader::Shader(const char* vertex_file, const char* fragment_file)
    : gl_shader(vertex_file, fragment_file) {
}

void Shader::activate() const {
    gl_shader.activate();
}

void Shader::set_uniform4f(const char* name, float v0, float v1, float v2, float v3) const {
    gl_shader.set_uniform4f(name, v0, v1, v2, v3);
}

void Shader::set_mat4(const char* name, const engine::math::Matrix4& mat) const {
    gl_shader.set_mat4(name, mat);
}

void Shader::set_vec3(const char* name, const engine::math::Vector& vec) const {
    gl_shader.set_vec3(name, vec);
}

void Shader::set_vec4(const char* name, const engine::math::Vector4& vec) const {
    gl_shader.set_vec4(name, vec);
}

void Shader::set_int(const char* name, int value) const {
    gl_shader.set_int(name, value);
}

}  // namespace engine::render
