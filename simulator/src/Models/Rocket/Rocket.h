#pragma once

#include <common/LinearAlgebra.h>

struct RocketState {
    vec3<double> position;
    vec3<double> velocity;
    vec3<double> acceleration;

    double mass;
    double csArea; // Cross-section area for drag calculations, top-down view
    double Cd = 0.8; // Drag coefficient
};

class RocketEngine {
public:
    vec2<Degrees<double>> engineAngle;

    /*
    TODO: derive diff equation for thrust, based on:
        - engine dimensions
        - time
    */
    vec3<double> getThrust();
};

class Rocket {
public:
    RocketState state;
    RocketEngine engine;
};