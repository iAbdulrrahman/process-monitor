#pragma once

#include <iostream>
#include <vector>

std::vector<std::string> list_dir(std::string path);

std::string open_file(std::string path);

std::vector<std::string> split_string(std::string str, std::string delimeter);

std::string format_size_from_kb(double sizeInKB);