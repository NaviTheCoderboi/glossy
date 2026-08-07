#include "terminal.hpp"

#include <sys/ioctl.h>
#include <unistd.h>

#include <cstdio>

namespace glossy {

Terminal::Terminal() { enableRawMode(); }

Terminal::~Terminal() {
    if (_alternateScreen) leaveAlternateScreen();

    disableRawMode();

    showCursor();
}

void Terminal::write(std::string_view data) {
    std::fwrite(data.data(), 1, data.size(), stdout);
}

void Terminal::flush() { std::fflush(stdout); }

void Terminal::enableRawMode() {
    if (_rawEnabled) return;

    if (tcgetattr(STDIN_FILENO, &_original) == -1) return;

    termios raw{_original};

    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= CS8;
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) return;

    _rawEnabled = true;
}

void Terminal::disableRawMode() {
    if (!_rawEnabled) return;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &_original);

    _rawEnabled = false;
}

void Terminal::enterAlternateScreen() {
    if (_alternateScreen) return;

    write("\x1b[?1049h");
    flush();

    _alternateScreen = true;
}

void Terminal::leaveAlternateScreen() {
    if (!_alternateScreen) return;

    write("\x1b[?1049l");
    flush();

    _alternateScreen = false;
}

void Terminal::hideCursor() {
    write("\x1b[?25l");
    flush();
};

void Terminal::showCursor() {
    write("\x1b[?25h");
    flush();
};

std::uint32_t Terminal::width() const {
    winsize ws{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

    return ws.ws_col;
}

std::uint32_t Terminal::height() const {
    winsize ws{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

    return ws.ws_row;
}

std::size_t Terminal::read(void* buffer, std::size_t size) {
    return ::read(STDIN_FILENO, buffer, size);
}

};  // namespace glossy