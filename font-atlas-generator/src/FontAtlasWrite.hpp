#pragma once

#include <fontatlas/fontatlas.hpp>

void writeGlyphDataFile(const std::string &filepath, const fontatlas::atlas_header &header, const std::unordered_map<uint32_t, fontatlas::glyph_data> &rows);
