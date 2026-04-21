#ifndef SHADER_HPP
#define SHADER_HPP

#include "../renderOpenGL/GLShaderProgram.hpp"

namespace engine::math {
class Matrix4;
class Vector;
class Vector4;
}

namespace engine::render {

class Shader final {
public:
    Shader(const char* vertex_file, const char* fragment_file);

    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;
    Shader(Shader&& other) noexcept = default;
    Shader& operator=(Shader&& other) noexcept = default;
    ~Shader() = default;

    void activate() const;

    void set_uniform4f(const char* name, float v0, float v1, float v2, float v3) const;
    void set_mat4(const char* name, const engine::math::Matrix4& mat) const;
    void set_vec3(const char* name, const engine::math::Vector& vec) const;
    void set_vec4(const char* name, const engine::math::Vector4& vec) const;
    void set_int(const char* name, int value) const;

private:
    opengl::GLShader gl_shader;
};

}  // namespace engine::render

#endif  // SHADER_HPP
