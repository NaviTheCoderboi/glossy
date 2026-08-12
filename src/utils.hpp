#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <concepts>
#include <ostream>
#include <type_traits>

namespace glossy {

template <class... Ts>
struct Overloaded : Ts... {
    using Ts::operator()...;
};

template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

namespace concepts 
{
template <typename T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;
}
template <concepts::Arithmetic T = float>
struct Vec2D {
    T x{};
    T y{};

    using FloatType =
        std::conditional_t<sizeof(T) <= sizeof(float), float, double>;

    constexpr Vec2D() = default;
    constexpr Vec2D(T x, T y) noexcept : x(x), y(y) {}

    constexpr auto operator+=(this auto&& self, const Vec2D& other) noexcept {
        self.x += other.x;
        self.y += other.y;
        return self;
    }

    constexpr auto operator-=(this auto&& self, const Vec2D& other) noexcept {
        self.x -= other.x;
        self.y -= other.y;
        return self;
    }

    constexpr auto operator*=(this auto&& self, T scalar) noexcept {
        self.x *= scalar;
        self.y *= scalar;
        return self;
    }

    constexpr T dot(const Vec2D& other) const noexcept {
        return x * other.x + y * other.y;
    }

    constexpr T lengthSquared() const noexcept { return x * x + y * y; }

    constexpr FloatType length() const noexcept {
        return std::sqrt(static_cast<FloatType>(lengthSquared()));
    }

    constexpr Vec2D<FloatType> normalized() const noexcept {
        if (FloatType len = length(); len > 0) {
            return {static_cast<FloatType>(x) / len,
                    static_cast<FloatType>(y) / len};
        }
        return {};
    }

    auto operator<=>(const Vec2D&) const = default;

    friend std::ostream& operator<<(std::ostream& os, const Vec2D& vec) {
        return os << "Vec2D(" << vec.x << ", " << vec.y << ")";
    }
};

template <concepts::Arithmetic T>
constexpr Vec2D<T> operator+(Vec2D<T> lhs, const Vec2D<T>& rhs) noexcept {
    lhs += rhs;
    return lhs;
}

template <concepts::Arithmetic T>
constexpr Vec2D<T> operator-(Vec2D<T> lhs, const Vec2D<T>& rhs) noexcept {
    lhs -= rhs;
    return lhs;
}

template <concepts::Arithmetic T>
constexpr Vec2D<T> operator*(Vec2D<T> vec, T scalar) noexcept {
    vec *= scalar;
    return vec;
}

template <concepts::Arithmetic T>
constexpr Vec2D<T> operator*(T scalar, Vec2D<T> vec) noexcept {
    vec *= scalar;
    return vec;
}

template <concepts::Arithmetic T>
constexpr Vec2D<T> max(const Vec2D<T>& a, const Vec2D<T>& b) noexcept {
    return {std::max(a.x, b.x), std::max(a.y, b.y)};
};

template <concepts::Arithmetic T>
constexpr Vec2D<T> min(const Vec2D<T>& a, const Vec2D<T>& b) noexcept {
    return {std::min(a.x, b.x), std::min(a.y, b.y)};
};

};  // namespace glossy

#endif  // UTILS_HPP
