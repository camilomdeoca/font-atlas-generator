#include "FontAtlasWrite.hpp"

#include <fstream>

void writeGlyphDataFile(const std::string &filepath, const fontatlas::header &header, const std::unordered_map<uint32_t, fontatlas::glyph_data> &rows)
{
    std::ofstream file(filepath, std::ios::binary);

    file.write(reinterpret_cast<const char*>(&header), sizeof(fontatlas::header));

    for (const auto &[charCode, glyphData] : rows)
    {
        fontatlas::glyph_data_row row(charCode, glyphData);
        file.write(reinterpret_cast<char*>(&row), sizeof(fontatlas::glyph_data_row));
    }
}
