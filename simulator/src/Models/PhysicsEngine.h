#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Rocket/Rocket.h"
#include "World/Atmosphere.h"
#include <common/Physics.h>

struct ForceResult {
    vec3<double> force;
    vec3<double> moment;
};

#ifdef DEBUG_MODE
struct ForceDebug {
    std::string_view name;
    ForceResult fr;
};
#endif

// RETURNS EVERYTHING IN BODY FRAME
class ForceGenerator {
public:
    virtual ~ForceGenerator() = default;

    virtual std::string_view name() const = 0;
    virtual ForceResult calculate(Rocket &rocket, double t) = 0;
};

class ForceGravity : public ForceGenerator {
public:
    std::string_view name() const override {
        return "Gravity";
    }

    ForceResult calculate(Rocket &rocket, double t) override {
        double Fg = -9.81;
        double mass = rocket.getMass();

        vec3<double> gWorld {0.0, 0.0, Fg * mass};
        vec3<double> gBody = rocket.state.rotation.inverseRotate(gWorld);

        ForceResult fr = {
            gBody,
            {0.0, 0.0, 0.0} // No momentum induced
        };

        return fr;
    }
};

class ForceDrag : public ForceGenerator {
public:
    std::string_view name() const override {
        return "Drag";
    }

    ForceResult calculate(Rocket &rocket, double t) override {
        vec3<double> wind {0.0, 0.0, 0.0}; //TODO: in the future, pass wind through environment data, instead of declaring it here
        vec3<double> relativeVelocityWorld = rocket.state.velocity - wind;
        vec3<double> relativeVelocityBody = rocket.state.rotation.inverseRotate(relativeVelocityWorld);
        double speed = relativeVelocityBody.length();

        return {
            relativeVelocityBody * (-0.5 * Atmosphere::getDensity(rocket.state.position.z()) * rocket.state.Cd * rocket.state.csArea * speed),
            {0.0, 0.0, 0.0} // No momentum induced
            };
    }
};

class ForceThrust : public ForceGenerator {
public:
    std::string_view name() const override {
        return "Thrust";
    }

    ForceResult calculate(Rocket &rocket, double t) override {
        vec3<double> Ft = rocket.getThrust(t);

        vec3<double> r = rocket.structure.engine.geometryProperties.position - rocket.getCenterOfMass();
        vec3<double> M = r.cross(Ft);

        return {
            Ft,
            M
        };
    }
};

class PhysicsEngine {
private:
    std::vector<std::unique_ptr<ForceGenerator>> forces;
public:
    PhysicsEngine();

    // Calculate forces, obviously in Newtons
    vec3<double> calculateGravity(Rocket &rocket) const;
    vec3<double> calculateDrag(const Rocket &rocket) const;

    void update(Rocket &rocket, double t, double dt);
};