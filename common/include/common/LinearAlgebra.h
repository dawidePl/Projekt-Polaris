#pragma once

#include <cstddef>
#include <array>
#include <cmath>
#include "Types.h"


template<typename T, std::size_t N>
class vec {
private:
    std::array<T, N> e{};

public:
    vec() = default;

    // Fill all components with one value
    explicit vec(T a) { e.fill(a); }

    // Convenience constructors for the common sizes
    vec(T a, T b) requires (N == 2) : e{a, b} {}
    vec(T a, T b, T c) requires (N == 3) : e{a, b, c} {}
    vec(T a, T b, T c, T d) requires (N == 4) : e{a, b, c, d} {}

    T& operator[](std::size_t i) { return e[i]; }
    const T& operator[](std::size_t i) const { return e[i]; }

    // Named accessors, enabled only where they make sense
    T x() const requires (N >= 1) { return e[0]; }
    T y() const requires (N >= 2) { return e[1]; }
    T z() const requires (N >= 3) { return e[2]; }
    T w() const requires (N >= 4) { return e[3]; }

    vec operator+(const vec& v) const {
        vec r;
        for (std::size_t i = 0; i < N; ++i) r[i] = e[i] + v[i];
        return r;
    }
    vec operator-(const vec& v) const {
        vec r;
        for (std::size_t i = 0; i < N; ++i) r[i] = e[i] - v[i];
        return r;
    }
    vec operator*(const vec& v) const {
        vec r;
        for (std::size_t i = 0; i < N; ++i) r[i] = e[i] * v[i];
        return r;
    }
    vec operator*(const T& s) const {
        vec r;
        for (std::size_t i = 0; i < N; ++i) r[i] = e[i] * s;
        return r;
    }
    vec operator/(const vec& v) const {
        vec r;
        for (std::size_t i = 0; i < N; ++i) r[i] = e[i] / v[i];
        return r;
    }
    vec operator/(const T& s) const {
        vec r;
        for (std::size_t i = 0; i < N; ++i) r[i] = e[i] / s;
        return r;
    }

    vec& operator+=(const vec& v) { for (std::size_t i=0;i<N;++i) e[i]+=v[i]; return *this; }
    vec& operator-=(const vec& v) { for (std::size_t i=0;i<N;++i) e[i]-=v[i]; return *this; }
    vec& operator*=(const vec& v) { for (std::size_t i=0;i<N;++i) e[i]*=v[i]; return *this; }
    vec& operator*=(const T& s)   { for (std::size_t i=0;i<N;++i) e[i]*=s;    return *this; }
    vec& operator/=(const vec& v) { for (std::size_t i=0;i<N;++i) e[i]/=v[i]; return *this; }
    vec& operator/=(const T& s)   { for (std::size_t i=0;i<N;++i) e[i]/=s;    return *this; }

    T dot(const vec& u) const {
        T sum{};
        for (std::size_t i = 0; i < N; ++i) sum += e[i] * u[i];
        return sum;
    }

    T length_squared() const { return dot(*this); }
    T length() const { return std::sqrt(length_squared()); }

    // Only defined for N == 3 — calling it on vec2/vec4 fails to compile
    vec cross(const vec& u) const requires (N == 3) {
        return { e[1]*u[2] - e[2]*u[1],
                 e[2]*u[0] - e[0]*u[2],
                 e[0]*u[1] - e[1]*u[0] };
    }
};

template<typename T> using vec2 = vec<T, 2>;
template<typename T> using vec3 = vec<T, 3>;
template<typename T> using vec4 = vec<T, 4>;


// Matrix implementation
template<typename T, std::size_t N>
class mat {
private:
    // Column-major: cols[j] is the j-th column vector
    std::array<vec<T, N>, N> cols{};

public:
    mat() = default;

    // Construct from N column vectors
    template<typename... Args>
    explicit mat(Args... args) requires (sizeof...(Args) == N)
        : cols{args...} {}

    static mat identity() {
        mat r;
        for (std::size_t i = 0; i < N; ++i) r.cols[i][i] = T(1);
        return r;
    }

    static mat zero() { return mat{}; }

    vec<T, N>& col(std::size_t j) { return cols[j]; }
    const vec<T, N>& col(std::size_t j) const { return cols[j]; }

    T& operator()(std::size_t row, std::size_t col_) { return cols[col_][row]; }
    const T& operator()(std::size_t row, std::size_t col_) const { return cols[col_][row]; }

    mat transpose() const {
        mat r;
        for (std::size_t i = 0; i < N; ++i)
            for (std::size_t j = 0; j < N; ++j)
                r(j, i) = (*this)(i, j);
        return r;
    }

    // mat * vec
    vec<T, N> operator*(const vec<T, N>& v) const {
        vec<T, N> r;
        for (std::size_t i = 0; i < N; ++i) {
            T sum{};
            for (std::size_t j = 0; j < N; ++j)
                sum += (*this)(i, j) * v[j];
            r[i] = sum;
        }
        return r;
    }

    // mat * mat
    mat operator*(const mat& o) const {
        mat r;
        for (std::size_t j = 0; j < N; ++j)
            r.cols[j] = (*this) * o.cols[j];
        return r;
    }

    mat operator+(const mat& o) const {
        mat r;
        for (std::size_t j = 0; j < N; ++j) r.cols[j] = cols[j] + o.cols[j];
        return r;
    }
    mat operator-(const mat& o) const {
        mat r;
        for (std::size_t j = 0; j < N; ++j) r.cols[j] = cols[j] - o.cols[j];
        return r;
    }
    mat operator*(const T& s) const {
        mat r;
        for (std::size_t j = 0; j < N; ++j) r.cols[j] = cols[j] * s;
        return r;
    }

    // --- Rotation factories, only meaningful for 3x3 ---
    static mat rotation_x(T angle) requires (N == 3) {
        T c = std::cos(angle), s = std::sin(angle);
        mat r = identity();
        r(1,1) = c;  r(1,2) = -s;
        r(2,1) = s;  r(2,2) = c;
        return r;
    }

    static mat rotation_y(T angle) requires (N == 3) {
        T c = std::cos(angle), s = std::sin(angle);
        mat r = identity();
        r(0,0) = c;  r(0,2) = s;
        r(2,0) = -s; r(2,2) = c;
        return r;
    }

    static mat rotation_z(T angle) requires (N == 3) {
        T c = std::cos(angle), s = std::sin(angle);
        mat r = identity();
        r(0,0) = c;  r(0,1) = -s;
        r(1,0) = s;  r(1,1) = c;
        return r;
    }

    // Rodrigues' formula: rotation about an arbitrary unit axis
    static mat rotation_axis_angle(const vec<T,3>& axis, Degrees<T> angle) requires (N == 3) {
        T c = std::cos(angle), s = std::sin(angle), t = T(1) - c;
        T x = axis.x(), y = axis.y(), z = axis.z();

        mat r;
        r(0,0) = t*x*x + c;    r(0,1) = t*x*y - s*z;  r(0,2) = t*x*z + s*y;
        r(1,0) = t*x*y + s*z;  r(1,1) = t*y*y + c;    r(1,2) = t*y*z - s*x;
        r(2,0) = t*x*z - s*y;  r(2,1) = t*y*z + s*x;  r(2,2) = t*z*z + c;
        return r;
    }

    // Determinant / inverse — specialized per size (2x2 and 3x3 closed forms)
    T determinant() const requires (N == 2) {
        return (*this)(0,0)*(*this)(1,1) - (*this)(0,1)*(*this)(1,0);
    }

    T determinant() const requires (N == 3) {
        return (*this)(0,0)*((*this)(1,1)*(*this)(2,2) - (*this)(1,2)*(*this)(2,1))
             - (*this)(0,1)*((*this)(1,0)*(*this)(2,2) - (*this)(1,2)*(*this)(2,0))
             + (*this)(0,2)*((*this)(1,0)*(*this)(2,1) - (*this)(1,1)*(*this)(2,0));
    }

    mat inverse() const requires (N == 2) {
        T d = determinant();
        mat r;
        r(0,0) =  (*this)(1,1) / d;  r(0,1) = -(*this)(0,1) / d;
        r(1,0) = -(*this)(1,0) / d;  r(1,1) =  (*this)(0,0) / d;
        return r;
    }

    mat inverse() const requires (N == 3) {
        T d = determinant();
        mat r;
        r(0,0) = ((*this)(1,1)*(*this)(2,2) - (*this)(1,2)*(*this)(2,1)) / d;
        r(0,1) = ((*this)(0,2)*(*this)(2,1) - (*this)(0,1)*(*this)(2,2)) / d;
        r(0,2) = ((*this)(0,1)*(*this)(1,2) - (*this)(0,2)*(*this)(1,1)) / d;
        r(1,0) = ((*this)(1,2)*(*this)(2,0) - (*this)(1,0)*(*this)(2,2)) / d;
        r(1,1) = ((*this)(0,0)*(*this)(2,2) - (*this)(0,2)*(*this)(2,0)) / d;
        r(1,2) = ((*this)(0,2)*(*this)(1,0) - (*this)(0,0)*(*this)(1,2)) / d;
        r(2,0) = ((*this)(1,0)*(*this)(2,1) - (*this)(1,1)*(*this)(2,0)) / d;
        r(2,1) = ((*this)(0,1)*(*this)(2,0) - (*this)(0,0)*(*this)(2,1)) / d;
        r(2,2) = ((*this)(0,0)*(*this)(1,1) - (*this)(0,1)*(*this)(1,0)) / d;
        return r;
    }

    // For a pure rotation matrix, inverse == transpose (much cheaper, no det/div)
    mat orthonormal_inverse() const { return transpose(); }
};

template<typename T> using mat2 = mat<T, 2>;
template<typename T> using mat3 = mat<T, 3>;
template<typename T> using mat4 = mat<T, 4>;