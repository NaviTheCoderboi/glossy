#ifndef ANSI_HPP
#define ANSI_HPP

#include <cstdint>
#include <string>

#include "buffer.hpp"

namespace glossy {

inline std::string cursorHome() { return "\x1b[H"; }

inline std::string clearScreen() { return "\x1b[2J"; }

inline std::string cursorTo(int32_t x, std::uint32_t y) {
    return "\x1b[" + std::to_string(y + 1) + ";" + std::to_string(x + 1) + "H";
}

inline std::string toUTF8(const std::u32string& str) {
    std::string utf8Str;
    for (char32_t ch : str) {
        if (ch <= 0x7F) {
            utf8Str += static_cast<char>(ch);
        } else if (ch <= 0x7FF) {
            utf8Str += static_cast<char>(0xC0 | ((ch >> 6) & 0x1F));
            utf8Str += static_cast<char>(0x80 | (ch & 0x3F));
        } else if (ch <= 0xFFFF) {
            utf8Str += static_cast<char>(0xE0 | ((ch >> 12) & 0x0F));
            utf8Str += static_cast<char>(0x80 | ((ch >> 6) & 0x3F));
            utf8Str += static_cast<char>(0x80 | (ch & 0x3F));
        } else {
            utf8Str += static_cast<char>(0xF0 | ((ch >> 18) & 0x07));
            utf8Str += static_cast<char>(0x80 | ((ch >> 12) & 0x3F));
            utf8Str += static_cast<char>(0x80 | ((ch >> 6) & 0x3F));
            utf8Str += static_cast<char>(0x80 | (ch & 0x3F));
        }
    }

    return utf8Str;
}

inline void renderFull(const Buffer& buffer) {
    std::string out{clearScreen() + cursorHome()};

    for (int32_t y{0}; y < buffer.height(); ++y) {
        out += cursorTo(0, y);

        for (int32_t x{0}; x < buffer.width(); ++x) {
            const Cell& cell{buffer.at(x, y)};

            if (cell.isPlaceholder()) continue;

            out += cell.content.empty() ? " " : toUTF8(cell.content);
        }

        if (y < buffer.height() - 1) {
            out += "\r\n";
        }
    }

    out += "\n";

    std::fwrite(out.data(), 1, out.size(), stdout);
    std::fflush(stdout);
}

};  // namespace glossy

#endif  // ANSI_HPP
