#pragma once

#include <iostream>
#include "Rocket/Rocket.h"
#include "World/Atmosphere.h"

class PhysicsEngine {
public:
    // Calculate forces, obviously in Newtons
    vec3<double> calculateGravity(const Rocket &rocket) const;
    vec3<double> calculateDrag(const Rocket &rocket) const;

    void update(Rocket &rocket, double dt);
};