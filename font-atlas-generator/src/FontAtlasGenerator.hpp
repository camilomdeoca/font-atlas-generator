#include <fontatlas/fontatlas.hpp>
#include <string>
#include <vector>

struct FontAtlas {
    std::vector<char> buffer;
    std::unordered_map<uint32_t, fontatlas::glyph_data> glyphsData;
};

FontAtlas generateFontAtlas(char *fontPath, int fontHeight, unsigned int atlasWidth, unsigned int atlasHeight, std::string charSet);
