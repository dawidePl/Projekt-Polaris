#pragma once

#include <cmath>
#include <common/LinearAlgebra.h>

class NoiseModel {
public:
    constexpr static double fract(double n) {
        return n - floor(n);
    }

    constexpr static double whiteNoise(double t) {
        vec2<double> x {t, t};
        vec2<double> a {12.9898, 78.233};

        return NoiseModel::fract(std::sin(x.dot(a)) * 4378.5453123);
    }

    constexpr static double randomGauss(double u, double sigma) {
        double u1 = NoiseModel::whiteNoise(sigma);
               u1 = (u1 == 0 ? 0.01 : u1);
        
        double u2 = NoiseModel::whiteNoise(sigma);

        double R = sqrt(-2.0 * log(u1));
        double o = 2 * 3.141592 * u2;
        double z0 = R * cos(o);

        return u + z0 * sigma;
    }
};