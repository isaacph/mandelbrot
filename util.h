#ifndef _UTIL_H
#define _UTIL_H
#include <png++/png.hpp>
#include <png++/rgba_pixel.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::string readFile(const std::string_view& file);
std::vector<unsigned char> readImage(const std::string_view& file, int& width, int& height);

#endif
