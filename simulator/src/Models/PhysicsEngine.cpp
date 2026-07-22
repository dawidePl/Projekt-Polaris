#include "PhysicsEngine.h"

vec3<double> PhysicsEngine::calculateGravity(Rocket &rocket) const {
    return {0.0, 0.0, rocket.getMass() * -9.81};
}

vec3<double> PhysicsEngine::calculateDrag(const Rocket &rocket) const {
    double drag = 0.5 * Atmosphere::getDensity(rocket.state.position.z()) * rocket.state.velocity.z() * rocket.state.velocity.z() * rocket.state.csArea * rocket.state.Cd;

    return {0.0, 0.0, drag * (rocket.state.velocity.z() > 0 ? -1 : 1)};
}

void PhysicsEngine::update(Rocket &rocket, double t, double dt) {
    #ifdef DEBUG_MODE
    std::vector<ForceDebug> forceDebug;
    #endif

    // Step 1. - burn fuel
    double mdot = rocket.engine.thrust / (rocket.engine.Isp * 9.80665);

    if(rocket.state.massFuel > mdot) {
        rocket.state.massFuel -= mdot;
    }else rocket.state.massFuel = 0;



    // Step 2. - calculate sum of forces acting on the rocket due to gravity z axis + is up, - is down
    std::vector<std::unique_ptr<ForceGenerator>> forces;

    forces.push_back(std::make_unique<ForceGravity>());
    forces.push_back(std::make_unique<ForceDrag>());
    forces.push_back(std::make_unique<ForceThrust>());
    
    vec3<double> sumForces {0.0};
    vec3<double> sumTorque {0.0};

    for(auto& force : forces) {
        ForceResult fr = force->calculate(rocket, t);

        sumForces += fr.force;
        sumTorque += fr.torque;

        #ifdef DEBUG_MODE
        ForceDebug fd = {
            force->name(),
            fr
        };

        forceDebug.push_back(fd);
        #endif
    }



    // Step 3.
    rocket.state.acceleration = sumForces / rocket.getMass();
    rocket.state.velocity += rocket.state.acceleration * dt;
    rocket.state.position += rocket.state.velocity * dt;


    // =========================== [ FOR DEBUGGING ] ===========================
    #ifdef DEBUG_MODE
        std::cout << "=========================== [ DEBUG ] ===========================" << std::endl;

        std::cout << "[FORCES]" << std::endl;
        
        for(auto &fd : forceDebug) {
            std::cout << fd.name << ": " << fd.fr.force << std::endl;
        }

        std::cout << std::endl << std::endl;

        std::cout << "[ROCKET STATE]" << std::endl;
        std::cout << "Position: " << rocket.state.position << std::endl;
        std::cout << "Velocity: " << rocket.state.velocity << std::endl;
        std::cout << "Acceleration: " << rocket.state.acceleration << std::endl;

        std::cout << "=========================== [ DEBUG ] ===========================" << std::endl << std::endl;
    #endif
    // =========================== [ FOR DEBUGGING ] ===========================
}