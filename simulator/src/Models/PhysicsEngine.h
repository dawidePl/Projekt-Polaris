#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Rocket/Rocket.h"
#include "World/Atmosphere.h"

struct ForceResult {
    vec3<double> force;
    vec3<double> torque;
};

class ForceGenerator {
public:
    virtual ~ForceGenerator() = default;

    virtual ForceResult calculate(Rocket &rocket) = 0;
};

class ForceGravity : public ForceGenerator {
public:
    ForceResult calculate(Rocket &rocket) override {
        double Fg = -9.81;
        double mass = rocket.state.massFuel + rocket.state.massStructural;

        ForceResult fr = {
            {0.0, 0.0, Fg * mass},
            {0.0, 0.0, 0.0} // TODO: change when implementing structural model
        };

        return fr;
    }
};

class ForceDrag : public ForceGenerator {
public:
    ForceResult calculate(Rocket &rocket) override {
        // double direction = 0.0;
        // double Fd = -0.5 * rocket.state.Cd * rocket.state.csArea * rocket.state.velocity.z() * rocket.state.velocity.z() * Atmosphere::getDensity(rocket.state.velocity.z());

        // return {
        //     {0.0, 0.0, Fd},
        //     {0.0, 0.0, 0.0}
        // };

        vec3<double> wind {0.0, 0.0, 0.0}; //TODO: in the future, pass wind through environment data, instead of declaring it here
        vec3<double> relativeVelocity = rocket.state.velocity - wind;
        double speed = relativeVelocity.length();

        return {
            relativeVelocity * (-0.5 * Atmosphere::getDensity(rocket.state.position.z()) * rocket.state.Cd * rocket.state.csArea * speed),
            {0.0, 0.0, 0.0}
            };
    }
};

class ForceThrust : public ForceGenerator {
public:
    ForceResult calculate(Rocket &rocket) override {
        vec3<double> Ft = rocket.engine.getThrust();

        return {
            Ft,
            {0.0, 0.0, 0.0}
        };
    }
};

class PhysicsEngine {
public:
    // Calculate forces, obviously in Newtons
    vec3<double> calculateGravity(Rocket &rocket) const;
    vec3<double> calculateDrag(const Rocket &rocket) const;

    void update(Rocket &rocket, double dt);
};