#ifndef FONT_HPP
#define FONT_HPP

#include "texture.hpp"

#include <unordered_map>

namespace engine::render {

struct Bounds {
    float left = 0.0f;
    float bottom = 0.0f;
    float right = 0.0f;
    float top = 0.0f;
};

struct Glyph {
    char32_t codepoint = 0;
    float advance = 0.0f;
    Bounds plane_bounds;
    Bounds atlas_bounds;
};

struct FontMetrics {
    float em_size = 0.0f;
    float line_height = 0.0f;
    float ascender = 0.0f;
    float descender = 0.0f;
    float underline_y = 0.0f;
    float underline_thickness = 0.0f;
};

struct FontAtlasInfo {
    int width = 0;
    int height = 0;
    float size = 0.0f;
    float distance_range = 0.0f;
    bool y_origin_bottom = true;
};

class Font {
public:
    Font(
        Texture atlas_texture,
        FontAtlasInfo atlas_info,
        FontMetrics metrics,
        std::unordered_map<char32_t, Glyph> glyphs
    );
    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;
    Font(Font&&) noexcept = default;
    Font& operator=(Font&&) noexcept = default;

    const Glyph* find_glyph(char32_t codepoint) const;

    const Texture& get_atlas_texture() const;
    const FontAtlasInfo& get_atlas_info() const;
    const FontMetrics& get_metrics() const;

private:
    Texture atlas_texture;
    FontAtlasInfo atlas_info;
    FontMetrics metrics;
    std::unordered_map<char32_t, Glyph> glyphs;
};

} // namespace engine::render

#endif // FONT_HPP
