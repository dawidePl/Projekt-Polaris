#pragma once

template<typename T> struct Degrees;

template<typename T> struct Radians;

template<typename T>
constexpr T deg_to_rad(T deg) {
    return deg * static_cast<T>(M_PI) / static_cast<T>(180);
}

template<typename T>
constexpr Degrees<T> rad_to_deg(T rad) {
    return rad * static_cast<T>(180) / static_cast<T>(M_PI);
}

template<typename T>
struct Degrees {
    T value;

    constexpr operator T() const { return value };
};

template<typename T>
struct Radians {
    T value;

    constexpr operator T() const { return value };
};