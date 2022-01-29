#ifndef _UTIL_H
#define _UTIL_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::string readFile(const std::string_view& file);
std::vector<unsigned char> readImage(const std::string_view& file, int& width, int& height);

#endif
