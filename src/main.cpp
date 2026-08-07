#include <string>
#include <thread>

#include "ansi.hpp"
#include "buffer.hpp"

void sleep(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int main() {
    glossy::Buffer buffer(10, 10);

    int progress{0};
    while (progress <= 10) {
        for (int i{0}; i < 10; ++i) {
            glossy::Cell newCell;
            newCell.content = (i < progress) ? U"█" : U"░";
            buffer.setCell(i, 5, newCell);
        }

        progress++;
        glossy::renderFull(buffer);

        sleep(200);
    }

    return 0;
}
