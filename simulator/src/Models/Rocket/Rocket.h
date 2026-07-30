#pragma once

#include <stdlib.h>
#include <vector>

#include <common/LinearAlgebra.h>
#include <common/Physics.h>

enum class GeometryType {
    UniformCylinder,
    HollowCylinder,
    SolidFrustum,
    HollowFrustum,
    Trapezoid,
    RectangularPrism
};

struct GeometryProperties {
    GeometryType type;

    vec3<double> position;

    double length;
    double mass;

    double diameter;
    double diameterTop;
    double diameterBottom;

    double innerRadius;
    double outerRadius;
};

struct PhysicalProperties {
    vec3<double> centerOfMass;
    vec3<double> centerOfPressure;
    mat3<double> inertiaTensor;
};

struct RocketState {
    vec3<double> position;
    quat<double> rotation;

    vec3<double> velocity;
    vec3<double> acceleration;

    vec3<double> angularVelocity;
    vec3<double> angularAcceleration;

    double csArea; // Cross-section area for drag calculations, top-down view
    double Cd = 0.8; // Drag coefficient
};

class RocketPart {
public:
    GeometryProperties geometryProperties;
    PhysicalProperties physicalProperties;

    vec3<double> position;
    quat<double> rotation;

    virtual ~RocketPart() = default;

    PhysicalProperties calculateGeometry() {
        GeometryProperties gp = this->geometryProperties;
        double r = gp.diameter / 2;

        switch(gp.type) {
            case GeometryType::UniformCylinder: {
                double Cx = 0.0;
                double Cy = 0.0;
                double Cz = gp.length / 2;
                
                double Ixx = (gp.mass / 12.0) * (3.0 * pow(r, 2) + pow(gp.length, 2));
                double Iyy = (gp.mass / 12.0) * (3.0 * pow(r, 2) + pow(gp.length, 2));
                double Izz = 0.5 * gp.mass * pow(r, 2);

                mat3<double> it (
                        {Ixx, 0, 0},
                        {0, Iyy, 0},
                        {0, 0, Izz}
                );

                return {
                    .centerOfMass = {Cx, Cy, Cz},
                    .inertiaTensor = it
                };
            };

            case GeometryType::HollowCylinder: {
                double Ri = gp.innerRadius;
                double Ro = gp.outerRadius;
                
                double z = gp.length / 2;

                double Ixx = (gp.mass / 12.0) * (3.0 * (pow(Ro, 2) + pow(Ri, 2)) + pow(gp.length, 2));
                double Iyy = (gp.mass / 12.0) * (3.0 * (pow(Ro, 2) + pow(Ri, 2)) + pow(gp.length, 2));
                double Izz = 0.5 * gp.mass * (pow(Ro, 2) + pow(Ri, 2));

                mat3<double> it (
                    {Ixx, 0.0, 0.0},
                    {0.0, Iyy, 0.0},
                    {0.0, 0.0, Izz}
                );

                return {
                    .centerOfMass = {0.0, 0.0, z},
                    .inertiaTensor = it
                };
            };
            
            case GeometryType::SolidFrustum: {
                // double z = 3 * gp.length / 4; // measuring from the top/tip of the frostum
                double z = gp.length / 4; // measuring from the bottom

                double Ixx = (3/80) * gp.mass * (4 * pow(r, 2) + pow(gp.length, 2));
                double Iyy = (3/80) * gp.mass * (4 * pow(r, 2) + pow(gp.length, 2));
                double Izz = (3/10) * gp.mass * pow(r, 2);

                mat3<double> it(
                    {Ixx, 0.0, 0.0},
                    {0.0, Iyy, 0.0},
                    {0.0, 0.0, Izz}
                );

                return {
                    .centerOfMass = {0.0, 0.0, z},
                    .inertiaTensor = it
                };
            };

            case GeometryType::HollowFrustum: {
                double z = 2 * gp.length / 3;

                double Ixx = gp.mass * ((pow(r, 2) / 4) + (pow(gp.length, 2) / 18));
                double Iyy = gp.mass * ((pow(r, 2) / 4) + (pow(gp.length, 2) / 18));
                double Izz = 0.5 * gp.mass * pow(r, 2);

                mat3<double> it(
                    {Ixx, 0.0, 0.0},
                    {0.0, Iyy, 0.0},
                    {0.0, 0.0, Izz}
                );

                return {
                    .centerOfMass = {0.0, 0.0, z},
                    .inertiaTensor = it
                };
            };

            default:
                return {};
        }
    }
};

class Engine : public RocketPart {
public:
    double thrust;
    double Isp;

    vec2<Radians<double>> engineAngle;
};

// Rocket Geometry
class Nose : public RocketPart {};

class BodyTube : public RocketPart {};

class FuelTank : public RocketPart {};

class Fin : public RocketPart {
public:
    double rootChord;
    double tipChord;
    double span;
    double sweep;
};

class RocketStructure {
public:
    Nose nose;
    BodyTube bodyTube;
    FuelTank fuelTank;
    std::vector<Fin> fins; // Assumes the same spacing between fins
    Engine engine;
};

class Rocket {
public:
    RocketState state;
    RocketStructure structure;

    void initialize();

    double getMass();
    vec3<double> getCenterOfMass();
    vec3<double> getCenterOfPressure();
    mat3<double> getInertiaTensor();

    double getBurnTime();
    void burnFuel(double dt);
    vec3<double> getThrust(double t);
};