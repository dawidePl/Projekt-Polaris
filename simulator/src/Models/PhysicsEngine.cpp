#include "PhysicsEngine.h"

vec3<double> PhysicsEngine::calculateGravity(Rocket &rocket) const {
    return {0.0, 0.0, rocket.getMass() * -9.81};
}

vec3<double> PhysicsEngine::calculateDrag(const Rocket &rocket) const {
    double drag = 0.5 * Atmosphere::getDensity(rocket.state.position.z()) * rocket.state.velocity.z() * rocket.state.velocity.z() * rocket.state.csArea * rocket.state.Cd;

    return {0.0, 0.0, drag * (rocket.state.velocity.z() > 0 ? -1 : 1)};
}

void PhysicsEngine::update(Rocket &rocket, double dt) {
    // Step 1. - calculate sum of forces acting on the rocket due to gravity z axis + is up, - is down
    // vec3 netForce = rocket.engine.getThrust() + this->calculateGravity(rocket) + this->calculateDrag(rocket);
    // vec3 Fgrav

    std::vector<std::unique_ptr<ForceGenerator>> forces;

    forces.push_back(std::make_unique<ForceGravity>());
    forces.push_back(std::make_unique<ForceDrag>());
    forces.push_back(std::make_unique<ForceThrust>());
    
    vec3<double> sumForces {0.0};

    for(auto& force : forces)
        sumForces += force->calculate(rocket).force;

    std::cout << "Net forces: (" << sumForces.x() << ", " << sumForces.y() << ", " << sumForces.z() << ")" << std::endl;

    // Step 2. - burn fuel
    if(rocket.state.massFuel > rocket.engine.massFlowRate * dt)
        rocket.state.massFuel -= rocket.engine.massFlowRate * dt;
    else if(rocket.state.massFuel > 0)
        rocket.state.massFuel = 0;
    else {
        rocket.state.massFuel = 0;
        rocket.engine.massFlowRate = 0;
    }

    // Step 3. - calculate acceleration
    // rocket.state.acceleration = netForce / rocket.getMass();
    // rocket.state.velocity += rocket.state.acceleration * dt;
    // rocket.state.position += rocket.state.velocity * dt;
}