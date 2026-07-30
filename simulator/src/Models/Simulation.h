#pragma once

#include "Rocket/Rocket.h"
#include "PhysicsEngine.h"
#include "World/World.h"

class Simulation {
private:
    double simuTime;
    double t;
    double dt;

    World world;
    PhysicsEngine physicsEngine;
    Rocket rocket;
    // IMU + FlightComputer
};