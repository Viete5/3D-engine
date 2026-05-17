#ifndef PATHS_HPP
#define PATHS_HPP

#include <filesystem>

namespace engine::core {

class Paths final {
public:
    void set_assets_dir(const std::filesystem::path& new_assets_dir);

    const std::filesystem::path& get_assets_dir() const;

    std::filesystem::path get_shader_path(const std::filesystem::path& relative_path) const;
    std::filesystem::path get_texture_path(const std::filesystem::path& relative_path) const;
    std::filesystem::path get_model_path(const std::filesystem::path& relative_path) const;
    std::filesystem::path get_scene_path(const std::filesystem::path& relative_path) const;

private:
    std::filesystem::path assets_dir_path;
};

}  // namespace engine::core

#endif  // PATHS_HPP
