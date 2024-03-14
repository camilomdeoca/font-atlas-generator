#include "FontAtlasGenerator.hpp"
#include "FontAtlasWrite.hpp"
#include <fstream>
#include <png++/ga_pixel.hpp>
#include <png++/png.hpp>
#include <png++/rgba_pixel.hpp>
#include <string>

void createFontAtlasFromFont(char *path, unsigned short fontHeight, char *outImage, std::string charSet, unsigned int atlasWidth, unsigned int atlasHeight)
{
    const unsigned int wantedChannels = 4;

    FontAtlas fontAtlas = generateFontAtlas(path, fontHeight, atlasWidth, atlasHeight, charSet);

    if (wantedChannels == 2) {
        png::image<png::ga_pixel> image(atlasWidth, atlasHeight);
        for (unsigned int y = 0; y < atlasHeight; y++) {
            for (unsigned int x = 0; x < atlasWidth; x++) {
                char val = fontAtlas.buffer[y * atlasWidth + x];
                image[y][x] = png::ga_pixel(val, val);
            }
        }
        image.write(outImage);
    } else if (wantedChannels == 4) {
        png::image<png::rgba_pixel> image(atlasWidth, atlasHeight);
        for (unsigned int y = 0; y < atlasHeight; y++) {
            for (unsigned int x = 0; x < atlasWidth; x++) {
                char val = fontAtlas.buffer[y * atlasWidth + x];
                image[y][x] = png::rgba_pixel(val, val, val, val);
            }
        }
        image.write(outImage);
    }

    std::ofstream file(std::string(outImage) + ".h");
    file << "#include <unordered_map>" << std::endl;
    file << std::endl;
    file << "struct Glyph" << std::endl;
    file << "{" << std::endl;
    file << "    struct ivec2 {" << std::endl;
    file << "        int x, y;" << std::endl;
    file << "    };" << std::endl;
    file << "    struct lvec2 {" << std::endl;
    file << "        long x, y;" << std::endl;
    file << "    };" << std::endl;
    file << "    struct uvec2 {" << std::endl;
    file << "        unsigned int x, y;" << std::endl;
    file << "    };" << std::endl;
    file << std::endl;
    file << "    ivec2 textureCoords;" << std::endl;
    file << "    uvec2 size;" << std::endl;
    file << "    lvec2 advance;" << std::endl;
    file << "    ivec2 offset;" << std::endl;
    file << "};" << std::endl;
    file << std::endl;
    file << "static std::unordered_map<char32_t, Glyph> fontData = {" << std::endl;
    for (auto [charCode, glyphData] : fontAtlas.glyphsData) {
        file << "\t{" << charCode << ", {";
        file << "{" << glyphData.offset.x << ", " << glyphData.offset.y << "}, ";
        file << "{" << glyphData.advance.x << ", " << glyphData.advance.y << "}, ";
        file << "{" << glyphData.texture_coords.x << ", " << glyphData.texture_coords.y << "}, ";
        file << "{" << glyphData.size.w << ", " << glyphData.size.h << "}}},";
        file << std::endl;
    }
    file << "};" << std::endl;
    file.close();

    fontatlas::atlas_header header{
        0,
        {atlasWidth, atlasHeight},
        static_cast<uint32_t>(fontAtlas.glyphsData.size()),
        fontHeight
    };

    writeGlyphDataFile(std::string(outImage) + ".fntat", header, fontAtlas.glyphsData);    
}

int main(int argc, char *argv[])
{
    if (argc < 6) {
        std::cout << "usage: " << argv[0] << " <input_font> <font_height> <output_png_image> <output_png_image_width> <output_png_image_height>" << std::endl;
        return 0;
    }

    std::string charSet = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ÁÉÍÓÚáéíóúñ";
    createFontAtlasFromFont(argv[1], std::stoul(argv[2]), argv[3], charSet, std::stoul(argv[4]), std::stoul(argv[5]));
    return 0;
}
