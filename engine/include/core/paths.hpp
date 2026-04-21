#ifndef PATH_HPP
#define PATH_HPP

#include <filesystem>

namespace engine::core {

class Paths final {
public:
    void set_executable_dir(const std::filesystem::path& new_executable_dir);
    void set_assets_dir(const std::filesystem::path& new_assets_dir);

    const std::filesystem::path& get_executable_dir() const;
    const std::filesystem::path& get_assets_dir() const;

    std::filesystem::path get_shader_path(const std::filesystem::path& relative_path) const;
    std::filesystem::path get_texture_path(const std::filesystem::path& relative_path) const;
    std::filesystem::path get_model_path(const std::filesystem::path& relative_path) const;
    std::filesystem::path get_scene_path(const std::filesystem::path& relative_path) const;

private:
    std::filesystem::path executable_dir_path;
    std::filesystem::path assets_dir_path;
};

}  // namespace engine::core

#endif  // PATHS_HPP
