#ifndef TEXT_MESH_BUILDER_HPP
#define TEXT_MESH_BUILDER_HPP

#include "font.hpp"
#include "mesh.hpp"
#include "../math/vector.hpp"
#include "../math/vector4.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace engine::render {

enum class TextHorizontalAlign {
    Left,
    Center,
    Right
};

enum class TextVerticalAlign {
    Baseline,
    Top,
    Center,
    Bottom
};

struct TextLayoutSettings {
    float font_size = 32.0f;
    float line_spacing = 1.0f;
    float letter_spacing = 0.0f;
    TextHorizontalAlign horizontal_align = TextHorizontalAlign::Left;
    TextVerticalAlign vertical_align = TextVerticalAlign::Baseline;
    engine::math::Vector4 color = engine::math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
};

struct TextMeshData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    engine::math::Vector size = engine::math::Vector(0.0f, 0.0f, 0.0f);
    std::size_t glyph_count = 0;
    std::size_t missing_glyph_count = 0;
};

class TextMeshBuilder {
public:
    TextMeshData build(
        const Font& font,
        const std::string& utf8_text,
        const TextLayoutSettings& settings
    ) const;

private:
    struct TextLine {
        std::vector<char32_t> codepoints;
        float width = 0.0f;
    };

    std::vector<char32_t> decode_utf8(const std::string& utf8_text) const;
    std::vector<TextLine> build_lines(
        const Font& font,
        const std::vector<char32_t>& codepoints,
        const TextLayoutSettings& settings,
        float scale,
        std::size_t& missing_glyph_count
    ) const;

    void append_line(
        TextMeshData& mesh_data,
        const Font& font,
        const TextLine& line,
        float cursor_y,
        float scale,
        const TextLayoutSettings& settings
    ) const;

    void append_glyph(
        TextMeshData& mesh_data,
        const Font& font,
        const Glyph& glyph,
        float cursor_x,
        float cursor_y,
        float scale,
        const engine::math::Vector4& color
    ) const;

    void append_quad_indices(TextMeshData& mesh_data, unsigned int first_vertex_index) const;
    float get_line_start_x(float line_width, float block_width, TextHorizontalAlign align) const;
    float get_block_start_y(float block_height, const FontMetrics& metrics, float scale, TextVerticalAlign align) const;
    float get_line_step(const FontMetrics& metrics, const TextLayoutSettings& settings, float scale) const;
};

} // namespace engine::render

#endif // TEXT_MESH_BUILDER_HPP
