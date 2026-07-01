#pragma once
#include <stdlib.h>

template<typename T>
class vec3 {
private:
    T e[3];
public:

    vec3() : e{0, 0, 0} {}
    vec3(T a, T b, T c) : e{a, b, c} {}
    vec3(T a) : e {a, a, a} {}

    T x() const;
    T y() const;
    T z() const;

    T operator[](size_t i);

    vec3 operator+(const vec3 &v);
    vec3 operator-(const vec3 &v);
    vec3 operator*(const vec3 &v);
    vec3 operator*(const T &i);
    
    T dot(const vec3 &u);
    T cross(const vec3 &u);
};