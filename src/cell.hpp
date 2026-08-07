#ifndef CELL_HPP
#define CELL_HPP

#include <cstdint>
#include <string>

namespace glossy {

// TODO: Implement style
class Style {
    friend bool operator==(const Style&, const Style&) = default;
};

struct Cell {
    std::u32string content;
    std::uint8_t width{1};
    std::int32_t ownerOffset{0};

    Style style;

    bool isPlaceholder() const { return ownerOffset != 0; };

    friend bool operator==(const Cell&, const Cell&) = default;
};

};  // namespace glossy

#endif  // CELL_HPP