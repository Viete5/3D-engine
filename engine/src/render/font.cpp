#include "../include/render/font.hpp"
#include <utility>

namespace engine::render {

Font::Font(
    Texture atlas_texture,
    FontAtlasInfo atlas_info,
    FontMetrics metrics,
    std::unordered_map<char32_t, Glyph> glyphs
)
    : atlas_texture(std::move(atlas_texture)),
      atlas_info(atlas_info),
      metrics(metrics),
      glyphs(std::move(glyphs)) {}

const Glyph* Font::find_glyph(char32_t codepoint) const {
    const auto found = glyphs.find(codepoint);

    if (found == glyphs.end()) {
        return nullptr;
    }

    return &found->second;
}

const Texture& Font::get_atlas_texture() const {
    return atlas_texture;
}

const FontAtlasInfo& Font::get_atlas_info() const {
    return atlas_info;
}

const FontMetrics& Font::get_metrics() const {
    return metrics;
}

} // namespace engine::render
