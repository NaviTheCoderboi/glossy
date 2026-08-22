#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <cstdint>
#include <vector>

#include "cell.hpp"

namespace glossy {

class Buffer {
   public:
    Buffer(std::uint32_t width, std::uint32_t height);

    std::uint32_t width() const;
    std::uint32_t height() const;

    const Cell& at(std::uint32_t x, std::uint32_t y) const;
    void setCell(uint32_t x, uint32_t y, const Cell& cell);
    void clearCell(uint32_t x, uint32_t y);
    void clear();

   private:
    void clearRune(std::uint32_t x, std::uint32_t y);
    void placeRune(std::uint32_t x, std::uint32_t y, const Cell& cell);
    inline std::size_t index(std::uint32_t x, std::uint32_t y) const;
    inline bool valid(std::uint32_t x, std::uint32_t y) const;

    std::uint32_t _width;
    std::uint32_t _height;
    std::vector<Cell> _cells;
};

};  // namespace glossy

#endif  // BUFFER_HPP