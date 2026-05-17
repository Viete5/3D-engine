#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include <string>

namespace engine::assets {

class TextureLoader {
public:
    TextureLoader(const char* image_path);
    TextureLoader(const unsigned char* data, int size, const char* source_name);
    TextureLoader(const TextureLoader& other) = delete;
    TextureLoader& operator=(const TextureLoader& other) = delete;
    TextureLoader(TextureLoader&& other) noexcept;
    TextureLoader& operator=(TextureLoader&& other) noexcept;
    ~TextureLoader();
    
    int get_img_width() const {return img_width;}
    int get_img_height() const {return img_height;}
    int get_img_num_col_channel() const {return img_num_col_channel;}
    const unsigned char* get_bytes() const {return bytes;}
    std::string get_error_message() const;
    bool is_loaded() const {return bytes;}

private:
    int img_width = 0;
    int img_height = 0;
    int img_num_col_channel = 0;
    unsigned char *bytes = nullptr;
    std::string image_path;
};

} // namespace engine::assets

#endif // TEXTURE_LOADER_HPP