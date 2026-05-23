#include "../include/assets/modelLoader.hpp"

#include "../include/core/log.hpp"
#include "../include/math/vector.hpp"
#include "../include/scene/transform.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace engine::assets {

namespace {

constexpr GLenum texture_type = GL_TEXTURE_2D;
constexpr GLenum texture_slot = GL_TEXTURE0;
constexpr GLenum texture_pixel_type = GL_UNSIGNED_BYTE;

struct ModelLoadStats {
    std::size_t mesh_count = 0;
    std::size_t material_count = 0;
    std::size_t texture_count = 0;
    std::size_t object_count = 0;
    std::size_t nodes_visited = 0;
    std::size_t embedded_texture_count = 0;
    std::size_t external_texture_count = 0;
    std::size_t reused_texture_count = 0;
    std::size_t missing_uv_mesh_count = 0;
    std::size_t missing_normal_mesh_count = 0;
};

unsigned int get_import_flags(const ModelLoaderSettings& settings) {
    unsigned int flags =
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_SortByPType |
        aiProcess_ValidateDataStructure;

    if (settings.flip_uvs) {
        flags |= aiProcess_FlipUVs;
    }

    if (settings.generate_smooth_normals) {
        flags |= aiProcess_GenSmoothNormals;
    }

    return flags;
}

engine::math::Vector to_vector(const aiVector3D& vector) {
    return engine::math::Vector(vector.x, vector.y, vector.z);
}

engine::math::Vector to_euler_xyz(const aiQuaternion& quaternion) {
    const float x = quaternion.x;
    const float y = quaternion.y;
    const float z = quaternion.z;
    const float w = quaternion.w;

    const float sin_roll_cos_pitch = 2.0f * (w * x + y * z);
    const float cos_roll_cos_pitch = 1.0f - 2.0f * (x * x + y * y);
    const float roll = std::atan2(sin_roll_cos_pitch, cos_roll_cos_pitch);

    const float sin_pitch = 2.0f * (w * y - z * x);
    const float pitch =
        std::abs(sin_pitch) >= 1.0f
            ? std::copysign(1.5707963267948966f, sin_pitch)
            : std::asin(sin_pitch);

    const float sin_yaw_cos_pitch = 2.0f * (w * z + x * y);
    const float cos_yaw_cos_pitch = 1.0f - 2.0f * (y * y + z * z);
    const float yaw = std::atan2(sin_yaw_cos_pitch, cos_yaw_cos_pitch);

    return engine::math::Vector(roll, pitch, yaw);
}

engine::scene::Transform to_transform(const aiMatrix4x4& matrix) {
    aiVector3D scale;
    aiQuaternion rotation;
    aiVector3D position;
    matrix.Decompose(scale, rotation, position);

    return engine::scene::Transform(
        to_vector(position),
        to_euler_xyz(rotation),
        to_vector(scale)
    );
}

std::filesystem::path normalize_path(const std::filesystem::path& path) {
    std::error_code error;
    const std::filesystem::path normalized = std::filesystem::weakly_canonical(path, error);
    return error ? path.lexically_normal() : normalized;
}

std::filesystem::path resolve_texture_path(
    const std::filesystem::path& model_dir,
    const aiString& texture_path
) {
    const std::filesystem::path raw_path(texture_path.C_Str());
    if (raw_path.is_absolute()) {
        return normalize_path(raw_path);
    }

    return normalize_path(model_dir / raw_path);
}

void apply_base_color(engine::render::Material& target_material, const aiMaterial& source_material) {
#ifdef AI_MATKEY_BASE_COLOR
    aiColor4D base_color;
    if (source_material.Get(AI_MATKEY_BASE_COLOR, base_color) == AI_SUCCESS) {
        target_material.set_base_color(engine::math::Vector(base_color.r, base_color.g, base_color.b));
        return;
    }
#endif

    aiColor3D diffuse_color;
    if (source_material.Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color) == AI_SUCCESS) {
        target_material.set_base_color(
            engine::math::Vector(diffuse_color.r, diffuse_color.g, diffuse_color.b)
        );
    }
}

bool get_base_color_texture_path(const aiMaterial& material, aiString& texture_path) {
    if (material.GetTexture(aiTextureType_BASE_COLOR, 0, &texture_path) == AI_SUCCESS) {
        return true;
    }

    return material.GetTexture(aiTextureType_DIFFUSE, 0, &texture_path) == AI_SUCCESS;
}

engine::render::Texture* load_embedded_texture(
    engine::render::Model& model,
    const aiScene& scene,
    const aiString& texture_path,
    ModelLoadStats& stats
) {
    const aiTexture* embedded_texture = scene.GetEmbeddedTexture(texture_path.C_Str());
    if (!embedded_texture) {
        return nullptr;
    }

    if (embedded_texture->mHeight != 0) {
        throw std::runtime_error(
            "ModelLoader does not support raw embedded textures yet: " +
            std::string(texture_path.C_Str())
        );
    }

    const auto* data = reinterpret_cast<const unsigned char*>(embedded_texture->pcData);
    const int size = static_cast<int>(embedded_texture->mWidth);

    ++stats.embedded_texture_count;
    ++stats.texture_count;

    return &model.create_texture_from_memory(
        data,
        size,
        texture_path.C_Str(),
        texture_type,
        texture_slot,
        texture_pixel_type
    );
}

engine::render::Texture* load_external_texture(
    engine::render::Model& model,
    const std::filesystem::path& model_dir,
    const aiString& texture_path,
    std::unordered_map<std::string, engine::render::Texture*>& texture_cache,
    ModelLoadStats& stats
) {
    const std::filesystem::path resolved_path = resolve_texture_path(model_dir, texture_path);
    const std::string cache_key = resolved_path.string();

    const auto found = texture_cache.find(cache_key);
    if (found != texture_cache.end()) {
        ++stats.reused_texture_count;
        return found->second;
    }

    engine::render::Texture& texture = model.create_texture(
        cache_key.c_str(),
        texture_type,
        texture_slot,
        texture_pixel_type
    );
    texture_cache.emplace(cache_key, &texture);
    ++stats.external_texture_count;
    ++stats.texture_count;

    return &texture;
}

engine::render::Texture* load_material_texture(
    engine::render::Model& model,
    const aiScene& scene,
    const aiMaterial& material,
    const std::filesystem::path& model_dir,
    std::unordered_map<std::string, engine::render::Texture*>& texture_cache,
    ModelLoadStats& stats
) {
    aiString texture_path;
    if (!get_base_color_texture_path(material, texture_path)) {
        return nullptr;
    }

    if (const aiTexture* embedded_texture = scene.GetEmbeddedTexture(texture_path.C_Str())) {
        (void)embedded_texture;
        return load_embedded_texture(model, scene, texture_path, stats);
    }

    return load_external_texture(model, model_dir, texture_path, texture_cache, stats);
}

engine::render::Mesh& load_mesh(engine::render::Model& model, const aiMesh& source_mesh, ModelLoadStats& stats) {
    std::vector<engine::render::Vertex> vertices;
    std::vector<unsigned int> indices;

    vertices.reserve(source_mesh.mNumVertices);

    for (unsigned int vertex_index = 0; vertex_index < source_mesh.mNumVertices; ++vertex_index) {
        const aiVector3D& position = source_mesh.mVertices[vertex_index];
        const bool has_normals = source_mesh.HasNormals();
        const aiVector3D normal = has_normals ? source_mesh.mNormals[vertex_index] : aiVector3D(0.0f, 1.0f, 0.0f);

        float tex_u = 0.0f;
        float tex_v = 0.0f;
        const bool has_uv = source_mesh.HasTextureCoords(0);
        if (has_uv) {
            tex_u = source_mesh.mTextureCoords[0][vertex_index].x;
            tex_v = source_mesh.mTextureCoords[0][vertex_index].y;
        }

        vertices.push_back(engine::render::Vertex{
            engine::math::Vector(position.x, position.y, position.z),
            engine::math::Vector(normal.x, normal.y, normal.z),
            tex_u,
            tex_v
        });
    }

    if (!source_mesh.HasNormals()) {
        ++stats.missing_normal_mesh_count;
    }

    if (!source_mesh.HasTextureCoords(0)) {
        ++stats.missing_uv_mesh_count;
    }

    for (unsigned int face_index = 0; face_index < source_mesh.mNumFaces; ++face_index) {
        const aiFace& face = source_mesh.mFaces[face_index];
        for (unsigned int index = 0; index < face.mNumIndices; ++index) {
            indices.push_back(face.mIndices[index]);
        }
    }

    ++stats.mesh_count;
    return model.create_mesh(vertices, indices);
}

engine::render::Material& load_material(
    engine::render::Model& model,
    const aiScene& scene,
    const aiMaterial& source_material,
    engine::render::Shader& shader,
    const std::filesystem::path& model_dir,
    const ModelLoaderSettings& settings,
    std::unordered_map<std::string, engine::render::Texture*>& texture_cache,
    ModelLoadStats& stats
) {
    engine::render::Material& material = model.create_material(shader);
    ++stats.material_count;
    apply_base_color(material, source_material);

    if (settings.load_textures) {
        if (engine::render::Texture* texture =
                load_material_texture(model, scene, source_material, model_dir, texture_cache, stats)) {
            material.set_base_color_texture(texture);
        }
    }

    return material;
}

engine::render::Material& get_material_for_mesh(
    const aiMesh& mesh,
    std::vector<engine::render::Material*>& materials,
    engine::render::Material& default_material
) {
    if (mesh.mMaterialIndex < materials.size() && materials[mesh.mMaterialIndex] != nullptr) {
        return *materials[mesh.mMaterialIndex];
    }

    return default_material;
}

void process_node(
    engine::render::Model& model,
    const aiScene& scene,
    const aiNode& node,
    const aiMatrix4x4& parent_transform,
    const std::vector<engine::render::Mesh*>& meshes,
    std::vector<engine::render::Material*>& materials,
    engine::render::Material& default_material,
    ModelLoadStats& stats
) {
    ++stats.nodes_visited;
    const aiMatrix4x4 global_transform = parent_transform * node.mTransformation;
    const engine::scene::Transform transform = to_transform(global_transform);

    for (unsigned int node_mesh_index = 0; node_mesh_index < node.mNumMeshes; ++node_mesh_index) {
        const unsigned int mesh_index = node.mMeshes[node_mesh_index];
        if (mesh_index >= meshes.size() || meshes[mesh_index] == nullptr) {
            continue;
        }

        const aiMesh& source_mesh = *scene.mMeshes[mesh_index];
        engine::render::Material& material = get_material_for_mesh(source_mesh, materials, default_material);
        model.add_object(node.mName.C_Str(), transform, material, *meshes[mesh_index]);
        ++stats.object_count;
    }

    for (unsigned int child_index = 0; child_index < node.mNumChildren; ++child_index) {
        process_node(
            model,
            scene,
            *node.mChildren[child_index],
            global_transform,
            meshes,
            materials,
            default_material,
            stats
        );
    }
}

void log_load_stats(
    engine::core::Logger* logger,
    const std::filesystem::path& model_path,
    const ModelLoadStats& stats
) {
    if (!logger) {
        return;
    }

    logger->info(
        engine::core::LogCategory::Assets,
        "Loaded model: " +
            model_path.string() +
            " | meshes=" + std::to_string(stats.mesh_count) +
            " materials=" + std::to_string(stats.material_count) +
            " textures=" + std::to_string(stats.texture_count) +
            " objects=" + std::to_string(stats.object_count) +
            " nodes=" + std::to_string(stats.nodes_visited)
    );

    if (stats.external_texture_count > 0 || stats.embedded_texture_count > 0 || stats.reused_texture_count > 0) {
        logger->info(
            engine::core::LogCategory::Assets,
            "Model textures: external=" + std::to_string(stats.external_texture_count) +
                " embedded=" + std::to_string(stats.embedded_texture_count) +
                " reused=" + std::to_string(stats.reused_texture_count)
        );
    }

    if (stats.missing_uv_mesh_count > 0) {
        logger->warning(
            engine::core::LogCategory::Assets,
            "Model meshes without UV: " + std::to_string(stats.missing_uv_mesh_count)
        );
    }

    if (stats.missing_normal_mesh_count > 0) {
        logger->warning(
            engine::core::LogCategory::Assets,
            "Model meshes without normals: " + std::to_string(stats.missing_normal_mesh_count)
        );
    }
}

} // namespace

ModelLoader::ModelLoader(const ModelLoaderSettings& settings)
    : settings(settings) {
}

ModelLoader::ModelLoader(const ModelLoaderSettings& settings, engine::core::Logger* logger)
    : settings(settings),
      logger(logger) {
}

engine::render::Model ModelLoader::load(
    const std::filesystem::path& model_path,
    engine::render::Shader& shader
) const {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(model_path.string(), get_import_flags(settings));
    if (!scene || !scene->mRootNode) {
        throw std::runtime_error(
            "ModelLoader failed to load model: " +
            model_path.string() +
            " " +
            importer.GetErrorString()
        );
    }

    engine::render::Model model;
    const std::filesystem::path model_dir = model_path.parent_path();
    std::unordered_map<std::string, engine::render::Texture*> texture_cache;
    ModelLoadStats stats;

    engine::render::Material& default_material = model.create_material(shader);
    ++stats.material_count;

    std::vector<engine::render::Mesh*> meshes(scene->mNumMeshes, nullptr);
    for (unsigned int mesh_index = 0; mesh_index < scene->mNumMeshes; ++mesh_index) {
        meshes[mesh_index] = &load_mesh(model, *scene->mMeshes[mesh_index], stats);
    }

    std::vector<engine::render::Material*> materials(scene->mNumMaterials, nullptr);
    for (unsigned int material_index = 0; material_index < scene->mNumMaterials; ++material_index) {
        materials[material_index] = &load_material(
            model,
            *scene,
            *scene->mMaterials[material_index],
            shader,
            model_dir,
            settings,
            texture_cache,
            stats
        );
    }

    aiMatrix4x4 identity;
    process_node(model, *scene, *scene->mRootNode, identity, meshes, materials, default_material, stats);
    log_load_stats(logger, model_path, stats);

    return std::move(model);
}

const ModelLoaderSettings& ModelLoader::get_settings() const {
    return settings;
}

void ModelLoader::set_settings(const ModelLoaderSettings& new_settings) {
    settings = new_settings;
}

void ModelLoader::set_logger(engine::core::Logger* new_logger) {
    logger = new_logger;
}

} // namespace engine::assets
