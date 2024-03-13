# Font Atlas generator for my game engine

It puts all wanted glyphs in a png image and saves the glyphs positions in the atlas, and other
useful data for rendering the font in a binary file, `<output_png_image>.fntat`, that can be red with `read_glyph_data_file()` function
in `fontatlas/fontatlas.hpp` and in a generated `<output_png_image>.h` file.

usage:
```
font-atlas-generator <input_font> <font_height> <output_png_image> <output_png_image_width> <output_png_image_height>
```
