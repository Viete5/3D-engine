#include "../include/render/textMeshBuilder.hpp"

#include <algorithm>
#include <stdexcept>

namespace engine::render {

namespace {

constexpr char32_t replacement_codepoint = U'?';

bool has_drawable_bounds(const Glyph& glyph) {
    return glyph.plane_bounds.left != glyph.plane_bounds.right &&
           glyph.plane_bounds.bottom != glyph.plane_bounds.top &&
           glyph.atlas_bounds.left != glyph.atlas_bounds.right &&
           glyph.atlas_bounds.bottom != glyph.atlas_bounds.top;
}

float get_texture_v(const FontAtlasInfo& atlas, float atlas_y) {
    const float normalized_y = atlas_y / static_cast<float>(atlas.height);
    return atlas.y_origin_bottom ? 1.0f - normalized_y : normalized_y;
}

} // namespace

TextMeshData TextMeshBuilder::build(
    const Font& font,
    const std::string& utf8_text,
    const TextLayoutSettings& settings
) const {
    if (settings.font_size <= 0.0f) {
        throw std::runtime_error("TextMeshBuilder font_size must be positive");
    }

    const float em_size = font.get_metrics().em_size > 0.0f ? font.get_metrics().em_size : 1.0f;
    const float scale = settings.font_size / em_size;
    std::vector<char32_t> codepoints = decode_utf8(utf8_text);

    TextMeshData mesh_data;
    std::vector<TextLine> lines = build_lines(
        font,
        codepoints,
        settings,
        scale,
        mesh_data.missing_glyph_count
    );

    float block_width = 0.0f;
    for (const TextLine& line : lines) {
        block_width = std::max(block_width, line.width);
    }

    const float line_step = get_line_step(font.get_metrics(), settings, scale);
    const float block_height = lines.empty()
        ? 0.0f
        : font.get_metrics().line_height * scale +
          static_cast<float>(lines.size() - 1) * line_step;

    float cursor_y = get_block_start_y(block_height, font.get_metrics(), scale, settings.vertical_align);
    for (const TextLine& line : lines) {
        TextLine aligned_line = line;
        aligned_line.width = line.width;

        const float line_start_x = get_line_start_x(line.width, block_width, settings.horizontal_align);
        TextMeshData line_mesh_data;
        append_line(line_mesh_data, font, aligned_line, cursor_y, scale, settings);

        for (Vertex& vertex : line_mesh_data.vertices) {
            vertex.position = vertex.position + engine::math::Vector(line_start_x, 0.0f, 0.0f);
        }

        const unsigned int vertex_offset = static_cast<unsigned int>(mesh_data.vertices.size());
        mesh_data.vertices.insert(
            mesh_data.vertices.end(),
            line_mesh_data.vertices.begin(),
            line_mesh_data.vertices.end()
        );

        for (unsigned int index : line_mesh_data.indices) {
            mesh_data.indices.push_back(vertex_offset + index);
        }

        mesh_data.glyph_count += line_mesh_data.glyph_count;
        cursor_y -= line_step;
    }

    mesh_data.size = engine::math::Vector(block_width, block_height, 0.0f);
    return mesh_data;
}

std::vector<char32_t> TextMeshBuilder::decode_utf8(const std::string& utf8_text) const {
    std::vector<char32_t> codepoints;

    for (std::size_t index = 0; index < utf8_text.size();) {
        const unsigned char first_byte = static_cast<unsigned char>(utf8_text[index]);

        if (first_byte < 0x80) {
            codepoints.push_back(static_cast<char32_t>(first_byte));
            ++index;
            continue;
        }

        char32_t codepoint = 0;
        std::size_t bytes_count = 0;
        if ((first_byte & 0xE0) == 0xC0) {
            codepoint = first_byte & 0x1F;
            bytes_count = 2;
        } else if ((first_byte & 0xF0) == 0xE0) {
            codepoint = first_byte & 0x0F;
            bytes_count = 3;
        } else if ((first_byte & 0xF8) == 0xF0) {
            codepoint = first_byte & 0x07;
            bytes_count = 4;
        } else {
            codepoints.push_back(replacement_codepoint);
            ++index;
            continue;
        }

        if (index + bytes_count > utf8_text.size()) {
            codepoints.push_back(replacement_codepoint);
            break;
        }

        bool valid_sequence = true;
        for (std::size_t offset = 1; offset < bytes_count; ++offset) {
            const unsigned char next_byte = static_cast<unsigned char>(utf8_text[index + offset]);
            if ((next_byte & 0xC0) != 0x80) {
                valid_sequence = false;
                break;
            }

            codepoint = (codepoint << 6) | (next_byte & 0x3F);
        }

        if (!valid_sequence) {
            codepoints.push_back(replacement_codepoint);
            ++index;
            continue;
        }

        codepoints.push_back(codepoint);
        index += bytes_count;
    }

    return codepoints;
}

std::vector<TextMeshBuilder::TextLine> TextMeshBuilder::build_lines(
    const Font& font,
    const std::vector<char32_t>& codepoints,
    const TextLayoutSettings& settings,
    float scale,
    std::size_t& missing_glyph_count
) const {
    std::vector<TextLine> lines(1);
    missing_glyph_count = 0;

    for (char32_t codepoint : codepoints) {
        if (codepoint == U'\r') {
            continue;
        }

        if (codepoint == U'\n') {
            lines.push_back(TextLine{});
            continue;
        }

        const Glyph* glyph = font.find_glyph(codepoint);
        if (!glyph) {
            glyph = font.find_glyph(replacement_codepoint);
            ++missing_glyph_count;
        }

        if (!glyph) {
            continue;
        }

        lines.back().codepoints.push_back(glyph->codepoint);
        lines.back().width += glyph->advance * scale + settings.letter_spacing;
    }

    return lines;
}

void TextMeshBuilder::append_line(
    TextMeshData& mesh_data,
    const Font& font,
    const TextLine& line,
    float cursor_y,
    float scale,
    const TextLayoutSettings& settings
) const {
    float cursor_x = 0.0f;

    for (char32_t codepoint : line.codepoints) {
        const Glyph* glyph = font.find_glyph(codepoint);
        if (!glyph) {
            continue;
        }

        append_glyph(mesh_data, font, *glyph, cursor_x, cursor_y, scale, settings.color);
        cursor_x += glyph->advance * scale + settings.letter_spacing;
    }
}

void TextMeshBuilder::append_glyph(
    TextMeshData& mesh_data,
    const Font& font,
    const Glyph& glyph,
    float cursor_x,
    float cursor_y,
    float scale,
    const engine::math::Vector4& color
) const {
    if (!has_drawable_bounds(glyph)) {
        return;
    }

    const FontAtlasInfo& atlas = font.get_atlas_info();
    if (atlas.width <= 0 || atlas.height <= 0) {
        throw std::runtime_error("TextMeshBuilder font atlas dimensions must be positive");
    }

    const float x0 = cursor_x + glyph.plane_bounds.left * scale;
    const float y0 = cursor_y + glyph.plane_bounds.bottom * scale;
    const float x1 = cursor_x + glyph.plane_bounds.right * scale;
    const float y1 = cursor_y + glyph.plane_bounds.top * scale;

    const float u0 = glyph.atlas_bounds.left / static_cast<float>(atlas.width);
    const float u1 = glyph.atlas_bounds.right / static_cast<float>(atlas.width);
    const float v_bottom = get_texture_v(atlas, glyph.atlas_bounds.bottom);
    const float v_top = get_texture_v(atlas, glyph.atlas_bounds.top);

    const unsigned int first_vertex_index = static_cast<unsigned int>(mesh_data.vertices.size());
    const engine::math::Vector normal(0.0f, 0.0f, 1.0f);

    mesh_data.vertices.push_back(Vertex{engine::math::Vector(x0, y1, 0.0f), normal, u0, v_top, color});
    mesh_data.vertices.push_back(Vertex{engine::math::Vector(x1, y1, 0.0f), normal, u1, v_top, color});
    mesh_data.vertices.push_back(Vertex{engine::math::Vector(x0, y0, 0.0f), normal, u0, v_bottom, color});
    mesh_data.vertices.push_back(Vertex{engine::math::Vector(x1, y0, 0.0f), normal, u1, v_bottom, color});

    append_quad_indices(mesh_data, first_vertex_index);
    ++mesh_data.glyph_count;
}

void TextMeshBuilder::append_quad_indices(TextMeshData& mesh_data, unsigned int first_vertex_index) const {
    mesh_data.indices.push_back(first_vertex_index);
    mesh_data.indices.push_back(first_vertex_index + 2);
    mesh_data.indices.push_back(first_vertex_index + 1);

    mesh_data.indices.push_back(first_vertex_index + 1);
    mesh_data.indices.push_back(first_vertex_index + 2);
    mesh_data.indices.push_back(first_vertex_index + 3);
}

float TextMeshBuilder::get_line_start_x(
    float line_width,
    float block_width,
    TextHorizontalAlign align
) const {
    switch (align) {
        case TextHorizontalAlign::Left:
            return 0.0f;
        case TextHorizontalAlign::Center:
            return (block_width - line_width) * 0.5f;
        case TextHorizontalAlign::Right:
            return block_width - line_width;
    }

    return 0.0f;
}

float TextMeshBuilder::get_block_start_y(
    float block_height,
    const FontMetrics& metrics,
    float scale,
    TextVerticalAlign align
) const {
    switch (align) {
        case TextVerticalAlign::Baseline:
            return 0.0f;
        case TextVerticalAlign::Top:
            return -metrics.ascender * scale;
        case TextVerticalAlign::Center:
            return block_height * 0.5f - metrics.ascender * scale;
        case TextVerticalAlign::Bottom:
            return block_height - metrics.ascender * scale;
    }

    return 0.0f;
}

float TextMeshBuilder::get_line_step(
    const FontMetrics& metrics,
    const TextLayoutSettings& settings,
    float scale
) const {
    return metrics.line_height * settings.line_spacing * scale;
}

} // namespace engine::render
