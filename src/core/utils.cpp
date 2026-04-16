#include <iostream>
#include <vector>
#include <filesystem>
#include <string>
#include <fstream>
#include <ranges>
#include <format>
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

std::vector<std::string> split_string(std::string str, std::string delimeter) {
    std::vector<std::string> splitted;
    for(const auto &word : std::views::split(str, delimeter)) {
        splitted.push_back(std::string(word.begin(), word.end()));
    }
    return splitted;
}

std::string format_size_from_kb(double sizeInKB) {
    constexpr double KB_PER_MB = 1000.0;
    constexpr double KB_PER_GB = KB_PER_MB * 1000.0;

    if (sizeInKB >= KB_PER_GB) {
        return std::format("{:.2f} GB", sizeInKB / KB_PER_GB);
    }

    return std::format("{:.2f} MB", sizeInKB / KB_PER_MB);
}

std::string format_size_from_bytes(double sizeInBytes) {
    constexpr double BYTES_PER_KB = 1000.0;
    constexpr double BYTES_PER_MB = BYTES_PER_KB * 1000.0;
    constexpr double BYTES_PER_GB = BYTES_PER_MB * 1000.0;

    if (sizeInBytes >= BYTES_PER_GB) {
        return std::format("{:.2f} GB", sizeInBytes / BYTES_PER_GB);
    } else if (sizeInBytes >= BYTES_PER_MB) {
        return std::format("{:.2f} MB", sizeInBytes / BYTES_PER_MB);
    } else if (sizeInBytes >= BYTES_PER_KB) {
        return std::format("{:.2f} KB", sizeInBytes / BYTES_PER_KB);
    }

    return std::format("{:.2f} Bytes", sizeInBytes);
}