#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <cstdint>
#include <string>
#include <variant>

#include "terminal.hpp"
#include "utils.hpp"

namespace glossy {

enum class Key {
    Unknown,

    Character,

    Enter,
    Escape,
    Backspace,
    Tab,

    Up,
    Down,
    Left,
    Right,

    Home,
    End,

    Insert,
    Delete,

    PageUp,
    PageDown,

    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12
};

enum class Modifier : std::uint8_t {
    None = 0,
    Ctrl = 1 << 0,
    Alt = 1 << 1,
    Shift = 1 << 2,
    Meta = 1 << 3
};

inline Modifier operator|(Modifier a, Modifier b);
inline bool hasModifier(Modifier mods, Modifier m);

struct KeyEvent {
    Key key{Key::Unknown};

    std::u32string text;

    Modifier modifiers{Modifier::None};

    bool ctrl() const;
    bool alt() const;
    bool shift() const;
    bool matches(char32_t ch) const;
};

struct ResizeEvent {
    std::uint32_t width{};
    std::uint32_t height{};
};

using Event = std::variant<KeyEvent, ResizeEvent>;

Event parseEvent(char ch);

class Reader {
   public:
    explicit Reader(Terminal& terminal);

    Event read();

   private:
    Terminal& terminal_;
};

template <typename... Handlers>
void visit(const Event& event, Handlers&&... handlers) {
    std::visit(
        Overloaded{std::forward<Handlers>(handlers)..., [](const auto&) {}},
        event);
}

};  // namespace glossy

#endif  // EVENTS_HPP