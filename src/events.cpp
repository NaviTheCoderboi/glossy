#include "events.hpp"

namespace glossy {

inline Modifier operator|(Modifier a, Modifier b) {
    return static_cast<Modifier>(static_cast<std::uint8_t>(a) |
                                 static_cast<std::uint8_t>(b));
}

inline bool hasModifier(Modifier mods, Modifier m) {
    return (static_cast<std::uint8_t>(mods) & static_cast<std::uint8_t>(m)) !=
           0;
}

bool KeyEvent::ctrl() const { return hasModifier(modifiers, Modifier::Ctrl); }

bool KeyEvent::alt() const { return hasModifier(modifiers, Modifier::Alt); }

bool KeyEvent::shift() const { return hasModifier(modifiers, Modifier::Shift); }

bool KeyEvent::matches(char32_t ch) const {
    return key == Key::Character && text.size() == 1 && text[0] == ch;
}

Event parseEvent(char ch) {
    KeyEvent ev;

    unsigned char uc{static_cast<unsigned char>(ch)};

    if (uc == '\r') {
        ev.key = Key::Enter;
        return ev;
    }

    if (uc == '\t') {
        ev.key = Key::Tab;
        return ev;
    }

    if (uc == 127) {
        ev.key = Key::Backspace;
        return ev;
    }

    if (uc == 27) {
        ev.key = Key::Escape;
        return ev;
    }

    if (uc >= 1 && uc <= 26) {
        ev.key = Key::Character;
        ev.modifiers = Modifier::Ctrl;
        ev.text.push_back(U'a' + (uc - 1));

        return ev;
    }

    ev.key = Key::Character;
    ev.text.push_back(static_cast<char32_t>(uc));

    return ev;
};

Reader::Reader(Terminal& terminal) : terminal_(terminal) {}

Event Reader::read() {
    char c;

    terminal_.read(&c, 1);

    return parseEvent(c);
}

};  // namespace glossy