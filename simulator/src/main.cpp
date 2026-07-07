#include <iostream>
#include "Models/PhysicsEngine.h"
#include "Models/Rocket/Rocket.h"
#include <common/Types.h>

int main() {
    RocketState rocketState = {
        .position = {0.0, 0.0, 0.0},
        .velocity = {0.0, 0.0, 0.0},
        .acceleration = {0.0, 0.0, 0.0},
        .mass = 15.0,
        .csArea = 1.0,
        .Cd = 0.8
    };

    Rocket rocket;
    rocket.state = rocketState;
    rocket.engine.engineAngle = {0.0, deg_to_rad(1.0)};
    
    PhysicsEngine physicsEngine;

    double simuTime = 10.0; // seconds
    double dt = 0.05;

    double apogee = 0.0;

    while(simuTime > 0) {
        physicsEngine.update(rocket, dt); // dt = 0.05 seconds

        // std::cout << "Altitude: " << rocket.state.position.z() << " m" << std::endl;
        // std::cout << "Velocity: " << rocket.state.velocity.z() << " m/s" << std::endl;
        // std::cout << "Acceleration: " << rocket.state.acceleration.z() << " m/s^2" << std::endl;
        // std::cout << std::endl << std::endl;

        if(rocket.state.position.z() > apogee)
            apogee = rocket.state.position.z();

            std::cout << "Thrust: (" << rocket.engine.getThrust().x() << ", " << rocket.engine.getThrust().y() << ", " << rocket.engine.getThrust().z() << ")" << std::endl;

        simuTime -= dt;
    }

    std::cout << "Apogeum: " << apogee << " m" << std::endl;

    return 0;
}