#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <termios.h>

#include <cstdint>
#include <string_view>

namespace glossy {

class Terminal {
   public:
    Terminal();
    ~Terminal();

    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;

    Terminal(Terminal&&) = delete;
    Terminal& operator=(Terminal&&) = delete;

    void enterAlternateScreen();
    void leaveAlternateScreen();

    void hideCursor();
    void showCursor();

    std::uint32_t width() const;
    std::uint32_t height() const;

    void write(std::string_view data);
    void flush();
    std::size_t read(void* buffer, std::size_t size);

   private:
    void enableRawMode();
    void disableRawMode();

    termios _original{};
    bool _rawEnabled{false};
    bool _alternateScreen{false};
};

};  // namespace glossy

#endif  // TERMINAL_HPP