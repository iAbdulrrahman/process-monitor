#include <iostream>
#include <vector>
#include <filesystem>
#include <string>
#include <fstream>
#include "utils.h"

namespace fs = std::filesystem;

std::vector<std::string> list_dir(std::string path) {
    std::vector<std::string> v = {};
    for(const auto &entry : fs::directory_iterator(path)) {
        v.push_back(entry.path().string());
    }
    return v;
}

std::string open_file(std::string path) {
    std::string fileContents;
    if(fs::exists(fs::path(path))) {
        std::ifstream file(path);
        if(file.is_open()) {
            std::string s;
            while (std::getline(file, s))
                fileContents += s += '\n';
        }
    }
    return fileContents;
}