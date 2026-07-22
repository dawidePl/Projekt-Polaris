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
        .csArea = 0.0079,
        .Cd = 0.8
    };

    Rocket rocket;
    rocket.state = rocketState;
    rocket.engine = {
        .engineAngle = { deg_to_rad(0.0), deg_to_rad(0.0) },
        .thrust = 125,
        .Isp = 150
    };
    
    PhysicsEngine physicsEngine;

    double simuTime = 200.0; // seconds
    double t = 0.0;
    double dt = 0.05;

    double nextPrint = 0.0;
    const double printInterval = 0.05;

    while(t <= simuTime) {
        physicsEngine.update(rocket, t, dt); // dt = 0.05 seconds

        if(t >= nextPrint) {
            #ifndef DEBUG_MODE
            std::cout << "[ TIME  " << t << " ]" << std::endl;
            std::cout << "Position: " << rocket.state.position << std::endl;
            std::cout << "Velocity: " << rocket.state.velocity << std::endl;
            std::cout << std::endl;
            #endif

            nextPrint += printInterval;
        }

        if(rocket.state.position.z() <= 0) {
            std::cout << "SIMULATION ENDED, REACHED GROUND" << std::endl;

            break;
        }

        t += dt;
    }

    // std::cout << "Apogeum: " << apogee << " m" << std::endl;

    return 0;
}