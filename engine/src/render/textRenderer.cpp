#include "../include/render/textRenderer.hpp"

#include "../include/math/matrix.hpp"
#include "../include/render/font.hpp"

#include <glad/glad.h>

#include <stdexcept>

namespace engine::render {

TextRenderer::TextRenderer(const char* vertex_shader_path, const char* fragment_shader_path)
    : shader(vertex_shader_path, fragment_shader_path),
      mesh(std::vector<Vertex>{}, std::vector<unsigned int>{}),
      mesh_builder() {
}

void TextRenderer::draw_text(
    const Font& font,
    const std::string& text,
    float x,
    float y,
    unsigned int viewport_width,
    unsigned int viewport_height,
    const TextLayoutSettings& settings
) {
    if (viewport_width == 0 || viewport_height == 0 || text.empty()) {
        return;
    }

    TextMeshData mesh_data = mesh_builder.build(font, text, settings);
    if (mesh_data.indices.empty()) {
        return;
    }

    mesh.update_data(mesh_data.vertices, mesh_data.indices);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    shader.activate();
    shader.set_mat4(
        "projection",
        engine::math::Matrix4::get_ortho(
            0.0f,
            static_cast<float>(viewport_width),
            0.0f,
            static_cast<float>(viewport_height),
            -1.0f,
            1.0f
        )
    );
    shader.set_mat4("model", engine::math::Matrix4::get_translate(x, y, 0.0f));
    shader.set_int("font_atlas", 0);
    shader.set_float("px_range", font.get_atlas_info().distance_range);

    font.get_atlas_texture().bind_to_unit(0);
    mesh.draw();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

} // namespace engine::render
