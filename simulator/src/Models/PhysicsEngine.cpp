#include "PhysicsEngine.h"

double PhysicsEngine::calculateGravity(const Rocket &rocket) const {
    return rocket.state.mass * -9.81;
}

double PhysicsEngine::calculateDrag(const Rocket &rocket) const {
    return 0.5 * Atmosphere::getDensity(rocket.state.position.z()) * rocket.state.velocity.z() * rocket.state.velocity.z() * rocket.state.csArea * rocket.state.Cd;
}

double PhysicsEngine::calculateThrust(const Rocket &rocket) const {
    return 10; // In the future calculate based on engine parameters, for now just hard-coded value
}

void PhysicsEngine::update(const Rocket &rocket, double dt) {
    // Step 1. - calculate sum of forces acting on the rocket + is up, - is down
    double netForce = this->calculateThrust(rocket) + this->calculateGravity(rocket) + this->calculateDrag(rocket);

    std::cout << "Net force: " << netForce << std::endl;
}