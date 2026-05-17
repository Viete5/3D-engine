#include "../include/assets/textureLoader.hpp"
#include "../include/assets/stb_image.h"
#include <stdexcept>

namespace engine::assets {

TextureLoader::TextureLoader(const char* image_path) 
    : image_path(image_path) {
    bytes = stbi_load(image_path, &img_width, &img_height, &img_num_col_channel, 0);
}

TextureLoader::TextureLoader(const unsigned char* data, int size, const char* source_name)
    : image_path(source_name ? source_name : "<memory>") {
    bytes = stbi_load_from_memory(data, size, &img_width, &img_height, &img_num_col_channel, 0);
}

TextureLoader::TextureLoader(TextureLoader&& other) noexcept 
    : img_height(other.img_height),
      img_width(other.img_width),
      img_num_col_channel(other.img_num_col_channel),
      bytes(other.bytes),
      image_path(other.image_path) {

    other.img_height = 0;
    other.img_num_col_channel = 0;
    other.img_width = 0;
    other.image_path = "";
    other.bytes = nullptr;
}

TextureLoader& TextureLoader::operator=(TextureLoader&& other) noexcept {
    if (&other == this) {
        return *this;
    }

    if (bytes != nullptr) {
        stbi_image_free(bytes);
    }

    img_height = other.img_height;
    img_num_col_channel = other.img_num_col_channel;
    img_width = other.img_width;
    bytes = other.bytes;
    image_path = other.image_path;

    other.img_height = 0;
    other.img_num_col_channel = 0;
    other.img_width = 0;
    other.image_path = "";
    other.bytes = nullptr;

    return *this;
}

TextureLoader::~TextureLoader() {
    stbi_image_free(bytes);
}

std::string TextureLoader::get_error_message() const {
    const char* reason = stbi_failure_reason();
    return std::string("ERROR::TEXTURE::FAILED_TO_LOAD_IMAGE: ") +
           image_path +
           " " +
           (reason ? reason : "unknown reason");
}

} // namespace engine::assets
