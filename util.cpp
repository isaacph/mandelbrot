#include "util.h"

std::string readFile(const std::string_view& file) {
    std::string output;
    
    std::ifstream fileHandle(file.data());
    std::string line;
    while (std::getline(fileHandle, line)) {
        output += line + "\n";
    }

    return output;
}

std::vector<unsigned char> readImage(const std::string_view& file, int& width, int& height) {
    png::image<png::rgba_pixel> image(file.data());
    width = image.get_width();
    height = image.get_height();
    std::vector<unsigned char> output(width * height * 4);
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            png::rgba_pixel pixel = image.get_pixbuf().get_pixel(j, i);
            output[(i * height + j) * 4 + 0] = pixel.red;
            output[(i * height + j) * 4 + 1] = pixel.green;
            output[(i * height + j) * 4 + 2] = pixel.blue;
            output[(i * height + j) * 4 + 3] = pixel.alpha;
        }
    }
    return output;
}
