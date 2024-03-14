#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace fontatlas {

struct atlas_header {
    uint32_t version;
    struct {
        uint32_t w, h;
    } atlas_size;
    uint32_t num_of_chars_in_atlas;
    uint16_t font_height;
    uint8_t pad[12];
};

struct glyph_data
{
    struct { int32_t x, y; } texture_coords;
    struct { uint16_t w, h; } size;
    struct { int16_t x, y; } advance;
    struct { int16_t x, y; } offset;
};

struct glyph_data_row {
    // UTF32 character code
    uint32_t char_code;
    glyph_data data;
};

std::pair<atlas_header, std::unordered_map<uint32_t, glyph_data>> read_glyph_data_file(const std::string &filepath);

}
