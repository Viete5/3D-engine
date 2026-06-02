#ifndef FONT_LOADER_HPP
#define FONT_LOADER_HPP

#include "../render/font.hpp"

#include <filesystem>

namespace engine::assets {

class FontLoader {
public:
    engine::render::Font load_msdf_font(
        const std::filesystem::path& atlas_path,
        const std::filesystem::path& metadata_path
    ) const;
};

} // namespace engine::assets

#endif // FONT_LOADER_HPP
