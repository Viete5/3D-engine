#include "../include/core/paths.hpp"

#include <filesystem>

namespace engine::core {

void Paths::set_assets_dir(const std::filesystem::path& new_assets_dir) {
    assets_dir_path = std::filesystem::weakly_canonical(new_assets_dir);
}

const std::filesystem::path& Paths::get_assets_dir() const {
    return assets_dir_path;
}

std::filesystem::path Paths::get_shader_path(const std::filesystem::path& relative_path) const {
    return assets_dir_path / "shaders" / relative_path;
}

std::filesystem::path Paths::get_texture_path(const std::filesystem::path& relative_path) const {
    return assets_dir_path / "textures" / relative_path;
}

std::filesystem::path Paths::get_model_path(const std::filesystem::path& relative_path) const {
    return assets_dir_path / "models" / relative_path;
}

std::filesystem::path Paths::get_scene_path(const std::filesystem::path& relative_path) const {
    return assets_dir_path / "scenes" / relative_path;
}

std::filesystem::path Paths::get_font_path(const std::filesystem::path& relative_path) const {
    return assets_dir_path / "fonts" / relative_path;
}

}  // namespace engine::core
