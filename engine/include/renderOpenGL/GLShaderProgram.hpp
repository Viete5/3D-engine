#ifndef GLSHADER_HPP
#define GLSHADER_HPP

#include <glad/glad.h>

namespace engine::math {
class Matrix4;
class Vector;
class Vector4;
}

namespace engine::render::opengl {

class GLShader {
public:
    // constructor reads and builds the shader
    GLShader(const char* , const char*);
    GLShader(const GLShader& other) = delete;
    GLShader& operator=(const GLShader& other) = delete;
    GLShader(GLShader&& other) noexcept;
    GLShader& operator=(GLShader&& other) noexcept;
    ~GLShader();

    // Uniforms
    void set_uniform4f(const char* , float, float, float , float ) const;
    void set_mat4(const char* name, const engine::math::Matrix4& mat) const;
    void set_vec3(const char* name, const engine::math::Vector& vec) const;
    void set_vec4(const char* name, const engine::math::Vector4& vec) const;
    void set_int(const char* name, int value) const;
    void set_float(const char* name, float value) const;

    // use/activate the shader
    void activate() const;
private:
    unsigned int id = 0;
};

} // namespace engine::render::opengl

#endif //SHADER_HPP
