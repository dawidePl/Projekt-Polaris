#pragma once

#include <common/LinearAlgebra.h>

struct RocketState {
    vec3<double> position;
    vec3<double> rotation;

    vec3<double> velocity;
    vec3<double> acceleration;

    double massFuel;
    double massStructural;
    double csArea; // Cross-section area for drag calculations, top-down view
    double Cd = 0.8; // Drag coefficient
};

class RocketEngine {
public:
    double thrust;
    double Isp;

    vec2<Radians<double>> engineAngle;

    /*
    TODO: derive diff equation for thrust, based on:
        - engine dimensions
        - time
    */
    // vec3<double> getThrust();
};

class Rocket {
public:
    RocketState state;
    RocketEngine engine;

    double getMass() {
        return this->state.massFuel + this->state.massStructural;
    }

    double getBurnTime();
    vec3<double> getThrust(double t);
};