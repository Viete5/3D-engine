#include "../include/assets/fontLoader.hpp"

#include "../include/render/texture.hpp"

#include <glad/glad.h>
#include <nlohmann/json.hpp>

#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace engine::assets {

namespace {

using Json = nlohmann::json;

const Json& require_field(const Json& object, const char* field_name, const char* object_name) {
    const auto found = object.find(field_name);
    if (found == object.end()) {
        throw std::runtime_error(
            std::string("FontLoader missing field '") +
            field_name +
            "' in " +
            object_name
        );
    }

    return *found;
}

float read_float(const Json& object, const char* field_name, const char* object_name) {
    const Json& value = require_field(object, field_name, object_name);
    if (!value.is_number()) {
        throw std::runtime_error(
            std::string("FontLoader field '") +
            field_name +
            "' in " +
            object_name +
            " must be a number"
        );
    }

    return value.get<float>();
}

int read_int(const Json& object, const char* field_name, const char* object_name) {
    const Json& value = require_field(object, field_name, object_name);
    if (!value.is_number_integer()) {
        throw std::runtime_error(
            std::string("FontLoader field '") +
            field_name +
            "' in " +
            object_name +
            " must be an integer"
        );
    }

    return value.get<int>();
}

engine::render::Bounds read_bounds(const Json& object) {
    return engine::render::Bounds{
        read_float(object, "left", "bounds"),
        read_float(object, "bottom", "bounds"),
        read_float(object, "right", "bounds"),
        read_float(object, "top", "bounds")
    };
}

engine::render::FontAtlasInfo read_atlas_info(const Json& metadata) {
    const Json& atlas = require_field(metadata, "atlas", "font metadata");

    engine::render::FontAtlasInfo atlas_info;
    atlas_info.width = read_int(atlas, "width", "atlas");
    atlas_info.height = read_int(atlas, "height", "atlas");
    atlas_info.size = read_float(atlas, "size", "atlas");
    atlas_info.distance_range = read_float(atlas, "distanceRange", "atlas");

    const Json& y_origin = require_field(atlas, "yOrigin", "atlas");
    if (!y_origin.is_string()) {
        throw std::runtime_error("FontLoader field 'yOrigin' in atlas must be a string");
    }
    atlas_info.y_origin_bottom = y_origin.get<std::string>() == "bottom";

    return atlas_info;
}

engine::render::FontMetrics read_metrics(const Json& metadata) {
    const Json& metrics = require_field(metadata, "metrics", "font metadata");

    return engine::render::FontMetrics{
        read_float(metrics, "emSize", "metrics"),
        read_float(metrics, "lineHeight", "metrics"),
        read_float(metrics, "ascender", "metrics"),
        read_float(metrics, "descender", "metrics"),
        read_float(metrics, "underlineY", "metrics"),
        read_float(metrics, "underlineThickness", "metrics")
    };
}

engine::render::Glyph read_glyph(const Json& glyph_data) {
    engine::render::Glyph glyph;
    glyph.codepoint = static_cast<char32_t>(read_int(glyph_data, "unicode", "glyph"));
    glyph.advance = read_float(glyph_data, "advance", "glyph");

    const auto plane_bounds = glyph_data.find("planeBounds");
    if (plane_bounds != glyph_data.end()) {
        glyph.plane_bounds = read_bounds(*plane_bounds);
    }

    const auto atlas_bounds = glyph_data.find("atlasBounds");
    if (atlas_bounds != glyph_data.end()) {
        glyph.atlas_bounds = read_bounds(*atlas_bounds);
    }

    return glyph;
}

std::unordered_map<char32_t, engine::render::Glyph> read_glyphs(const Json& metadata) {
    const Json& glyphs_data = require_field(metadata, "glyphs", "font metadata");
    if (!glyphs_data.is_array()) {
        throw std::runtime_error("FontLoader field 'glyphs' must be an array");
    }

    std::unordered_map<char32_t, engine::render::Glyph> glyphs;
    glyphs.reserve(glyphs_data.size());

    for (const Json& glyph_data : glyphs_data) {
        engine::render::Glyph glyph = read_glyph(glyph_data);
        glyphs.emplace(glyph.codepoint, glyph);
    }

    return glyphs;
}

Json read_metadata_file(const std::filesystem::path& metadata_path) {
    std::ifstream file(metadata_path);
    if (!file.is_open()) {
        throw std::runtime_error("FontLoader failed to open metadata file: " + metadata_path.string());
    }

    Json metadata;
    file >> metadata;
    return metadata;
}

} // namespace

engine::render::Font FontLoader::load_msdf_font(
    const std::filesystem::path& atlas_path,
    const std::filesystem::path& metadata_path
) const {
    const Json metadata = read_metadata_file(metadata_path);

    engine::render::Texture atlas_texture(
        atlas_path.string().c_str(),
        GL_TEXTURE_2D,
        GL_TEXTURE0,
        GL_UNSIGNED_BYTE
    );

    engine::render::FontAtlasInfo atlas_info = read_atlas_info(metadata);
    engine::render::FontMetrics metrics = read_metrics(metadata);
    std::unordered_map<char32_t, engine::render::Glyph> glyphs = read_glyphs(metadata);

    return engine::render::Font(
        std::move(atlas_texture),
        atlas_info,
        metrics,
        std::move(glyphs)
    );
}

} // namespace engine::assets
