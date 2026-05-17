#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include "../render/model.hpp"
#include "../render/shader.hpp"

#include <filesystem>

namespace engine::assets {

struct ModelLoaderSettings {
    bool flip_uvs = true;
    bool generate_smooth_normals = true;
    bool load_textures = true;
};

class ModelLoader final {
public:
    ModelLoader() = default;
    explicit ModelLoader(const ModelLoaderSettings& settings);

    engine::render::Model load(
        const std::filesystem::path& model_path,
        engine::render::Shader& shader
    ) const;

    const ModelLoaderSettings& get_settings() const;
    void set_settings(const ModelLoaderSettings& new_settings);

private:
    ModelLoaderSettings settings;
};

} // namespace engine::assets

#endif // MODEL_LOADER_HPP
