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

class Rocket {
public:
    RocketState state;
};