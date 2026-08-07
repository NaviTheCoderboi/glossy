#ifndef ANSI_HPP
#define ANSI_HPP

#include <cstdint>
#include <string>

#include "buffer.hpp"

namespace glossy {

std::string cursorHome();
std::string clearScreen();
std::string cursorTo(std::uint32_t x, std::uint32_t y);

std::string toUTF8(const std::u32string& str);

void renderFull(const Buffer& buffer);

};  // namespace glossy

#endif  // ANSI_HPP