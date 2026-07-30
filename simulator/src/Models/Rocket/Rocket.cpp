#include "Rocket.h"

void Rocket::initialize() {
    this->structure.nose.physicalProperties = structure.nose.calculateGeometry();

    this->structure.bodyTube.physicalProperties = structure.bodyTube.calculateGeometry();

    this->structure.fuelTank.physicalProperties = structure.fuelTank.calculateGeometry();

    this->structure.engine.physicalProperties = structure.engine.calculateGeometry();

    for(auto& fin : structure.fins)
        fin.physicalProperties = fin.calculateGeometry();
}


double Rocket::getMass() {
    double finMass = this->structure.fins.size() * this->structure.fins.at(0).geometryProperties.mass;

    return this->structure.nose.geometryProperties.mass +
           this->structure.fuelTank.geometryProperties.mass +
           this->structure.engine.geometryProperties.mass +
           this->structure.bodyTube.geometryProperties.mass +
           finMass;
}

vec3<double> Rocket::getCenterOfMass() {
    vec3<double> finCoM {0.0, 0.0, 0.0};

    for(const auto &fin : this->structure.fins)
        finCoM += fin.physicalProperties.centerOfMass;
    
    return finCoM +
           this->structure.nose.physicalProperties.centerOfMass +
           this->structure.fuelTank.physicalProperties.centerOfMass +
           this->structure.engine.physicalProperties.centerOfMass +
           this->structure.bodyTube.physicalProperties.centerOfMass;
}

mat3<double> Rocket::getInertiaTensor() {
    mat3<double> finIT(
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}
    );

    for(const auto &fin : this->structure.fins)
        finIT += fin.physicalProperties.inertiaTensor;
    
    return finIT +
           this->structure.nose.physicalProperties.inertiaTensor +
           this->structure.fuelTank.physicalProperties.inertiaTensor +
           this->structure.engine.physicalProperties.inertiaTensor +
           this->structure.bodyTube.physicalProperties.inertiaTensor;
}

double Rocket::getBurnTime() {
    double mdot = this->structure.engine.thrust / (this->structure.engine.Isp * Physics::G0);

    return this->structure.fuelTank.geometryProperties.mass / mdot;
}

void Rocket::burnFuel(double dt) {
    double mdot = this->structure.engine.thrust / (this->structure.engine.Isp * Physics::G0);
    double burnLen = this->structure.fuelTank.geometryProperties.length / this->getBurnTime() * dt;

    if(this->structure.fuelTank.geometryProperties.mass > mdot * dt) {
        this->structure.fuelTank.geometryProperties.mass -= mdot * dt;
        this->structure.fuelTank.geometryProperties.length -= burnLen;
        this->structure.fuelTank.calculateGeometry();
    }else {
        this->structure.fuelTank.geometryProperties.mass = 0;
        this->structure.fuelTank.geometryProperties.length = 0;
    }
}

// TODO: loading thrust data from OpenRocket data
vec3<double> Rocket::getThrust(double t) {
    mat3<double> R = mat3<double>::rotation_y(this->structure.engine.engineAngle.y()) * mat3<double>::rotation_x(this->structure.engine.engineAngle.x());
    vec3<double> Ft = {0.0, 0.0, this->structure.engine.thrust};

    if(this->structure.fuelTank.geometryProperties.mass <= 0)
        Ft = {0.0, 0.0, 0.0};

    return R * Ft;
}
