#include "FontAtlasGenerator.hpp"

#include <codecvt>
#include <ft2build.h>
#include <locale>
#include <string>
#include FT_FREETYPE_H
#include <stdexcept>

FontAtlas generateFontAtlas(char *fontPath, int fontHeight, unsigned int atlasWidth, unsigned int atlasHeight, std::string charSet)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        throw std::runtime_error("Error initializing FreeType library.");

    FT_Face face;
    if (FT_New_Face(ft, fontPath, 0, &face))
        throw std::runtime_error("Error loading font.");

    if (FT_Set_Pixel_Sizes(face, 0, fontHeight))
        throw std::runtime_error("Error setting font size.");

    FontAtlas fontAtlas;
    fontAtlas.buffer.resize(atlasWidth * atlasHeight, 0);

    std::u32string u32charSet = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().from_bytes(charSet);

    int padding = 1;
    int col = padding, row = 0;
    for (size_t i = 0; i < u32charSet.size(); i++) {
        FT_ULong charCode = u32charSet[i];
        
        FT_UInt glyphIndex = FT_Get_Char_Index(face, charCode);
        if (glyphIndex == 0)
            throw std::runtime_error("Invalid char index." + std::to_string(charCode));

        if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT))
            throw std::runtime_error("Error loading glyph.");

        if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))
            throw std::runtime_error("Error rendering glyph.");

        if (col + face->glyph->bitmap.width + padding >= atlasWidth) {
            col = padding;
            row += fontHeight + padding;
        }

        if ((face->size->metrics.ascender - face->size->metrics.descender) >> 6 > fontHeight) {
            fontHeight = (face->size->metrics.ascender - face->size->metrics.descender) >> 6;
        }

        for (unsigned int y = 0; y < face->glyph->bitmap.rows; y++) {
            for (unsigned int x = 0; x < face->glyph->bitmap.width; x++) {
                char val = face->glyph->bitmap.buffer[y * face->glyph->bitmap.width + x];
                fontAtlas.buffer[(row + y)*atlasWidth + col+x] = val;
            }
        }

        fontAtlas.glyphsData.insert(std::pair<uint32_t, fontatlas::glyph_data>(charCode, fontatlas::glyph_data{
            {col, row},
            {static_cast<uint16_t>(face->glyph->bitmap.width), static_cast<uint16_t>(face->glyph->bitmap.rows)},
            {static_cast<int16_t>(face->glyph->advance.x >> 6), static_cast<int16_t>(face->glyph->advance.y >> 6)},
            {static_cast<int16_t>(face->glyph->bitmap_left), static_cast<int16_t>(face->glyph->bitmap_top)}
        }));

        col += face->glyph->bitmap.width + padding;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return fontAtlas;
}
