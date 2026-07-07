#include "PhysicsEngine.h"

vec3<double> PhysicsEngine::calculateGravity(const Rocket &rocket) const {
    return {0.0, 0.0, rocket.state.mass * -9.81};
}

vec3<double> PhysicsEngine::calculateDrag(const Rocket &rocket) const {
    double drag = 0.5 * Atmosphere::getDensity(rocket.state.position.z()) * rocket.state.velocity.z() * rocket.state.velocity.z() * rocket.state.csArea * rocket.state.Cd;

    return {0.0, 0.0, drag * (rocket.state.velocity.z() > 0 ? -1 : 1)};
}

void PhysicsEngine::update(Rocket &rocket, double dt) {
    // Step 1. - calculate sum of forces acting on the rocket + is up, - is down
    vec3 netForce = rocket.engine.getThrust() + this->calculateGravity(rocket) + this->calculateDrag(rocket);

    std::cout << "Net force: " << netForce.z() << " N" << std::endl;

    // Step 2. - calculate acceleration
    rocket.state.acceleration = netForce / rocket.state.mass;
    rocket.state.velocity += rocket.state.acceleration * dt;
    rocket.state.position += rocket.state.velocity * dt;   
}