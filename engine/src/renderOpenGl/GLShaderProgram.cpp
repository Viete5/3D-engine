#include "../include/renderOpenGL/GLShaderProgram.hpp"
#include "../include/math/matrix.hpp"
#include "../include/math/vector.hpp"
#include "../include/math/vector4.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace engine::render::opengl {

namespace {

// Reading file
std::string get_file_contents(const char *filename)
{
  std::FILE *fp = std::fopen(filename, "rb");
  if (fp)
  {
    std::string contents;
    std::fseek(fp, 0, SEEK_END);
    contents.resize(std::ftell(fp));
    std::rewind(fp);
    std::fread(&contents[0], 1, contents.size(), fp);
    std::fclose(fp);
    return(contents);
  }
  throw std::runtime_error(std::string("Failed to open file: ") + filename);
}

} // namespace

GLShader::GLShader(const char *vertex_file, const char *fragment_file) {
    std::string vertex_code = get_file_contents(vertex_file);
    std::string fragment_code = get_file_contents(fragment_file);

    const char *vertex_source = vertex_code.c_str();
    const char *fragment_source = fragment_code.c_str();

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);
    // check for shader compile errors
    int success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        glDeleteShader(vertex_shader);
        throw std::runtime_error(std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n") + info_log);
    }
    // fragment shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);
    // check for shader compile errors
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        throw std::runtime_error(std::string("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n") + info_log);
    }
    // link shaders
    id = glCreateProgram();
    glAttachShader(id, vertex_shader);
    glAttachShader(id, fragment_shader);
    glLinkProgram(id);
    // check for linking errors
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, info_log);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        throw std::runtime_error(std::string("ERROR::SHADER::PROGRAM::LINKING_FAILED\n") + info_log);
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

GLShader::GLShader(GLShader&& other) noexcept
    : id(other.id) {
    other.id = 0;
}

GLShader& GLShader::operator=(GLShader&& other) noexcept {
    if (&other == this) {
        return *this;
    }

    if (id != 0) {
        glDeleteProgram(id);
    }

    id = other.id;
    other.id = 0;

    return *this;
}

GLShader::~GLShader() {
    if (id != 0) {
        glDeleteProgram(id);
    }    
}

// Set Vec4 uniform
void GLShader::set_uniform4f(const char* name, float v0, float v1, float v2, float v3) const{
    int location = glGetUniformLocation(id, name);
    glUniform4f(location, v0, v1, v2, v3);
}

// Set Mat4 uniform
void GLShader::set_mat4(const char* name, const engine::math::Matrix4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_TRUE, &mat.at(0,0));
}

// Set Vec3 uniform
void GLShader::set_vec3(const char* name, const engine::math::Vector& vec) const {
    glUniform3f(glGetUniformLocation(id, name), vec.get_x(), vec.get_y(), vec.get_z());
}

//Set int uniform
void GLShader::set_int(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(id, name), value);
}

void GLShader::set_float(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(id, name), value);
}

// Set Vec3 uniform
void GLShader::set_vec4(const char* name, const engine::math::Vector4& vec) const {
    glUniform4f(glGetUniformLocation(id, name), vec.get_x(), vec.get_y(), vec.get_z(), vec.get_w());
}

void GLShader::activate() const{
    glUseProgram(id);
}

} // namespace engine::render::opengl
