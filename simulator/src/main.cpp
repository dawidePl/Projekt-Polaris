#include <iostream>
#include "Models/PhysicsEngine.h"
#include "Models/Rocket/Rocket.h"
#include <common/Types.h>
#include "Noise/NoiseModel.h"

int main() {
    RocketState rocketState = {
        .position = {0.0, 0.0, 0.0},
        .velocity = {0.0, 0.0, 0.0},
        .acceleration = {0.0, 0.0, 0.0},
        .massFuel = 5.0,
        .massStructural = 3.5,
        .csArea = 1.0,
        .Cd = 0.8
    };

    Rocket rocket;
    rocket.state = rocketState;
    rocket.engine = {
        .engineAngle = { deg_to_rad(0.0), deg_to_rad(0.0) },
        .massFlowRate = 1.0,
        .Ve = 1250
    };
    
    PhysicsEngine physicsEngine;

    double simuTime = 10.0; // seconds
    double dt = 0.05;

    double apogee = 0.0;

    while(simuTime > 0) {
        physicsEngine.update(rocket, dt); // dt = 0.05 seconds

        simuTime -= dt;
    }

    std::cout << "Apogeum: " << apogee << " m" << std::endl;

    return 0;
}