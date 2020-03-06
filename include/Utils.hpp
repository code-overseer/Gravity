#ifndef GRAVITY_UTILS_HPP
#define GRAVITY_UTILS_HPP
#include <string>
#include <fstream>

inline std::string readFile(char const* path) {
    std::ifstream file(path);
    if (file.fail()) { throw std::runtime_error( "Could not open file"); }
    std::string text;
    file.seekg(0, std::ios::end);
    text.reserve(((size_t)file.tellg()) + 1);
    file.seekg(0, std::ios::beg);
    text.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    file.close();
    return text;
}

#endif //GRAVITY_UTILS_HPP
