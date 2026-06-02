#ifndef TEXT_RENDERER_HPP
#define TEXT_RENDERER_HPP

#include "mesh.hpp"
#include "shader.hpp"
#include "textMeshBuilder.hpp"

#include <string>
#include <vector>

namespace engine::render {

class Font;

class TextRenderer {
public:
    TextRenderer(const char* vertex_shader_path, const char* fragment_shader_path);

    TextRenderer(const TextRenderer&) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;
    TextRenderer(TextRenderer&&) noexcept = default;
    TextRenderer& operator=(TextRenderer&&) noexcept = default;

    void draw_text(
        const Font& font,
        const std::string& text,
        float x,
        float y,
        unsigned int viewport_width,
        unsigned int viewport_height,
        const TextLayoutSettings& settings
    );

private:
    Shader shader;
    Mesh mesh;
    TextMeshBuilder mesh_builder;
};

} // namespace engine::render

#endif // TEXT_RENDERER_HPP
