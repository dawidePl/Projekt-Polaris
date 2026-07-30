#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() {
    forces.push_back(std::make_unique<ForceGravity>());
    forces.push_back(std::make_unique<ForceDrag>());
    forces.push_back(std::make_unique<ForceThrust>());
}

vec3<double> PhysicsEngine::calculateGravity(Rocket &rocket) const {
    return {0.0, 0.0, rocket.getMass() * -9.81};
}

vec3<double> PhysicsEngine::calculateDrag(const Rocket &rocket) const {
    double drag = 0.5 * Atmosphere::getDensity(rocket.state.position.z()) * rocket.state.velocity.z() * rocket.state.velocity.z() * rocket.state.csArea * rocket.state.Cd;

    return {0.0, 0.0, drag * (rocket.state.velocity.z() > 0 ? -1 : 1)};
}

void PhysicsEngine::update(Rocket &rocket, double t, double dt) {
    // Step 1. - burn fuel
    rocket.burnFuel(dt);

    // // Step 2. - calculate sum of forces acting on the rocket due to gravity z axis + is up, - is down
    
    vec3<double> sumForcesBody {0.0};
    vec3<double> sumMomentsBody {0.0};

    for(auto& force : forces) {
        ForceResult fr = force->calculate(rocket, t);

        sumForcesBody += fr.force;
        sumMomentsBody += fr.moment;
    }

    vec3<double> forceWorld = rocket.state.rotation.rotate(sumForcesBody);



    // Step 3.
    rocket.state.acceleration = forceWorld / rocket.getMass();
    rocket.state.velocity += rocket.state.acceleration * dt;
    rocket.state.position += rocket.state.velocity * dt;

    mat3<double> I = rocket.getInertiaTensor();
    vec3<double> w = rocket.state.angularVelocity;
    
    std::cout << "Moment: "
          << sumMomentsBody.x() << " "
          << sumMomentsBody.y() << " "
          << sumMomentsBody.z()
          << std::endl;

    std::cout << "Angular acc: "
          << rocket.state.angularAcceleration
          << std::endl;
        
    std::cout << "Inertia tensor:\n"
          << I << std::endl;

    std::cout << "Inverse:\n"
          << I.inverse() << std::endl;

    rocket.state.angularAcceleration = I.inverse() * (sumMomentsBody - w.cross(I * w));
    rocket.state.angularVelocity += rocket.state.angularAcceleration * dt;
    
    w = rocket.state.angularVelocity;

    quat qOmega = {0.0, w.x(), w.y(), w.z()};
    quat qDot = 0.5 * (rocket.state.rotation * qOmega);

    rocket.state.rotation += qDot * dt;
    rocket.state.rotation.normalize();
}