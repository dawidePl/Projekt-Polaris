#include "../include/common/LinearAlgebra.h"

template<typename T>
T vec3<T>::x() const { return this->e[0]; };

template<typename T>
T vec3<T>::y() const { return this->e[1]; };

template<typename T>
T vec3<T>::z() const { return this->e[2]; };


template<typename T>
T vec3<T>::operator[](size_t i) {
    if(i < 0 || i > 2)
        return T{};

    return this->e[i];
};

template<typename T>
vec3<T> vec3<T>::operator+(const vec3<T> &v) {
    return {this->e[0] + v[0], this->e[1] + v[1], this->e[2] + v[2] };
};

template<typename T>
vec3<T> vec3<T>::operator-(const vec3<T> &v) {
    return {this->e[0] - v[0], this->e[1] - v[1], this->e[2] - v[2] };
};

template<typename T>
vec3<T> vec3<T>::operator*(const vec3<T> &v) {
    return {this->e[0] * v[0], this->e[1] * v[1], this->e[2] * v[2] };
};

template<typename T>
vec3<T> vec3<T>::operator*(const T &i) {
    return {this->e[0] * i, this->e[1] * i, this->e[2] * i};
};