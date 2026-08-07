#include "buffer.hpp"

namespace glossy {

Buffer::Buffer(std::uint32_t width, std::uint32_t height)
    : _width(width), _height(height), _cells(width * height) {}

std::uint32_t Buffer::width() const { return _width; }
std::uint32_t Buffer::height() const { return _height; }

const Cell& Buffer::at(std::uint32_t x, std::uint32_t y) const {
    return _cells[y * _width + x];
}

void Buffer::setCell(uint32_t x, uint32_t y, const Cell& cell) {
    if (!valid(x, y)) return;

    clearRune(x, y);

    if (cell.width > 1) {
        for (uint32_t i = 1; i < cell.width; ++i) clearRune(x + i, y);
    }

    placeRune(x, y, cell);
}

void Buffer::clearCell(uint32_t x, uint32_t y) { clearRune(x, y); }

void Buffer::clear() { std::fill(_cells.begin(), _cells.end(), Cell{}); }

void Buffer::clearRune(std::uint32_t x, std::uint32_t y) {
    if (!valid(x, y)) return;

    Cell& cell{_cells[index(x, y)]};
    if (cell == Cell{}) return;

    std::uint32_t ownerX{x};
    if (cell.isPlaceholder()) ownerX = x + cell.ownerOffset;

    Cell& owner{_cells[index(ownerX, y)]};

    for (std::uint32_t i = 0; i < owner.width; ++i)
        _cells[index(ownerX + i, y)] = Cell{};
}

void Buffer::placeRune(std::uint32_t x, std::uint32_t y, const Cell& cell) {
    if (!valid(x, y)) return;

    if (x + cell.width > _width) return;

    _cells[index(x, y)] = cell;

    for (std::uint32_t i = 1; i < cell.width; ++i) {
        _cells[index(x + i, y)] =
            Cell{.width = 0, .ownerOffset = -static_cast<std::int32_t>(i)};
    }
}

inline std::size_t Buffer::index(std::uint32_t x, std::uint32_t y) const {
    return static_cast<std::size_t>(y) * _width + x;
}

inline bool Buffer::valid(std::uint32_t x, std::uint32_t y) const {
    return x < _width && y < _height;
}

};  // namespace glossy