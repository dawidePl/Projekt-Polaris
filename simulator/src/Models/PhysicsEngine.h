#pragma once

#include <iostream>
#include "Rocket.h"
#include "World/Atmosphere.h"

class PhysicsEngine {
public:
    // Calculate forces, obviously in Newtons
    double calculateGravity(const Rocket &rocket) const;
    double calculateDrag(const Rocket &rocket) const;
    double calculateThrust(const Rocket &rocket) const;

    void update(const Rocket &rocket, double dt);
};