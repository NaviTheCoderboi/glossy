#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <cstdint>
#include <vector>

#include "cell.hpp"

namespace glossy {

        class Buffer {
           public:
            Buffer(int32_t width, uint32_t height)
                : _width(width), _height(height), _cells(width * height) {}

                int32_t width() const { return _width; }
                int32_t height() const { return _height; }

            const Cell& at(int32_t x, uint32_t y) const {
                return _cells[y * _width + x];
            }

            void setCell(int32_t x, uint32_t y, const Cell& cell) {
                if (x >= _width || y >= _height) return;

                if (_cells[index(x, y)].isPlaceholder()) {
                    int32_t ownerX{x + _cells[index(x, y)].ownerOffset};

                    _cells[index(ownerX, y)] = Cell{};
                }

                if (cell.width > 1) {
                    if (x + cell.width > _width) {
                        _cells[index(x, y)] = Cell{};
                        return;
                    }

                    _cells[index(x, y)] = cell;

                    for (int32_t i{1}; i < cell.width; ++i) {
                        _cells[index(x + i, y)] =
                            Cell{.ownerOffset = static_cast<int32_t>(-i)};
                    }
                } else {
                    _cells[index(x, y)] = cell;
                }
            }

            void clear() { std::fill(_cells.begin(), _cells.end(), Cell{}); }

           private:
            std::size_t index(int32_t x, uint32_t y) const {
                return static_cast<std::size_t>(y) * _width + x;
            }

            int32_t _width;
            int32_t _height;
            std::vector<Cell> _cells;
        };

};  // namespace glossy

#endif BUFFER_HPP // BUFFER_HPP
