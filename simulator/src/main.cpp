#include <iostream>
#include "Models/PhysicsEngine.h"
#include "Models/Rocket.h"

int main() {
    RocketState rocketState = {
        .position = {0.0, 0.0, 0.0},
        .velocity = {0.0, 0.0, 0.0},
        .acceleration = {0.0, 0.0, 0.0},
        .mass = 100.0,
        .csArea = 1.0,
        .Cd = 0.8
    };

    Rocket rocket;
    rocket.state = rocketState;

    PhysicsEngine physicsEngine;

    physicsEngine.update(rocket, 0.1); // Update for 0.1 seconds

    return 0;
}