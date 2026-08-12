#include <deque>

#include "ansi.hpp"
#include "buffer.hpp"
#include "events.hpp"
#include "terminal.hpp"

int main() {
    glossy::Terminal terminal;
    glossy::Reader reader(terminal);
    glossy::Buffer buffer(terminal.width(), terminal.height());

    terminal.enterAlternateScreen();
    terminal.hideCursor();

    std::deque<glossy::Vec2D<std::uint32_t>> snake;

    for (int i = 0; i < buffer.width(); ++i) {
        snake.push_back(
            {std::max(15u, buffer.width() / 2) - i, buffer.height() / 2});
    }

    int dx = 1;
    int dy = 0;

    bool running{true};
    while (running) {
        buffer.clear();

        for (size_t i = 0; i < snake.size(); ++i) {
            buffer.setCell(snake[i].x, snake[i].y, {i == 0 ? U"@" : U"O"});
        }

        glossy::renderFull(buffer);

        auto event = reader.read();

        glossy::visit(event, [&](const glossy::KeyEvent& e) {
            if (e.matches(U'q')) running = false;

            if (e.matches(U'w') && dy != 1) {
                dx = 0;
                dy = -1;
            } else if (e.matches(U's') && dy != -1) {
                dx = 0;
                dy = 1;
            } else if (e.matches(U'a') && dx != 1) {
                dx = -1;
                dy = 0;
            } else if (e.matches(U'd') && dx != -1) {
                dx = 1;
                dy = 0;
            }
        });

        glossy::Vec2D<std::uint32_t> head = snake.front();

        if (dx == 1)
            head.x = (head.x + 1) % buffer.width();
        else if (dx == -1)
            head.x = (head.x == 0) ? buffer.width() - 1 : head.x - 1;

        if (dy == 1)
            head.y = (head.y + 1) % buffer.height();
        else if (dy == -1)
            head.y = (head.y == 0) ? buffer.height() - 1 : head.y - 1;

        snake.push_front(head);
        snake.pop_back();
    }

    terminal.showCursor();
    terminal.leaveAlternateScreen();

    return 0;
}
