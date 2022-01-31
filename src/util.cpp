#include "util.h"
#include <cstdio>
#include <SOIL/SOIL.h>
#define PNG_DEBUG 3
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::string readFile(const std::string_view& file) {
    std::string output;
    
    std::ifstream fileHandle(file.data());
    std::string line;
    while (std::getline(fileHandle, line)) {
        output += line + "\n";
    }

    return output;
}

std::vector<unsigned char> readImage(const std::string_view& fileName, int& width, int& height) {
//    png::image<png::rgba_pixel> image(file.data());
//    width = image.get_width();
//    height = image.get_height();
//    std::vector<unsigned char> output(width * height * 4);
//    for (int i = 0; i < width; ++i) {
//        for (int j = 0; j < height; ++j) {
//            png::rgba_pixel pixel = image.get_pixbuf().get_pixel(j, i);
//            output[(i * height + j) * 4 + 0] = pixel.red;
//            output[(i * height + j) * 4 + 1] = pixel.green;
//            output[(i * height + j) * 4 + 2] = pixel.blue;
//            output[(i * height + j) * 4 + 3] = pixel.alpha;
//        }
//    }

//    return output;
    //width = -1;
    //height = -1;

    //FILE* file = fopen(fileName.data(), "r");
    //unsigned char sig[8];
    //fread(sig, 1, 8, file);
    //if (!png_check_sig(sig, 8)) {
    //    throw std::runtime_error(std::string("Failed to read png: ") + std::string(fileName));
    //}

    //struct png_struct* png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    //if (!png_ptr) {
    //    throw std::runtime_error(std::string("Failed to read png: ") + std::string(fileName));
    //}

    //png_infop info_ptr = png_create_info_struct(png_ptr);
    //if (!info_ptr) {
    //    throw std::runtime_error(std::string("Failed to read png: ") + std::string(fileName));
    //}

    //(struct png_struct)
    //if (setjmp(png_ptr->jmpbuf)) {
    //    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    //    throw std::runtime_error(std::string("Failed to read png: ") + std::string(fileName));
    //}
    int nrChannels;
    unsigned char *data = stbi_load(fileName.data(), &width, &height, &nrChannels, 0); 
    return std::vector<unsigned char>(data, data + width * height * nrChannels);
//
//    return {
//        255, 0, 0, 255,
//        0, 255, 0, 255,
//        0, 255, 0, 255,
//        255, 0, 0, 255
//    };
}

int divRoundDown(int a, int b) {
    if (a * b < 0) {
        return (a + 1) / b - 1;
    }
    return a / b;
}

int modRoundDown(int a, int b) {
    assert (b > 0);
    return ((a % b) + b) % b;
}
