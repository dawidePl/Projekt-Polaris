#include <iostream>

#include <common/Types.h>
#include "Models/PhysicsEngine.h"
#include "Models/Rocket/Rocket.h"
#include "Noise/NoiseModel.h"
#include "Logger/Logger.h"

int main() {
    RocketState rocketState = {
        .position = {0.0, 0.0, 0.0},
        .rotation = {1.0, 0.0, 0.0, 0.0},

        .velocity = {0.0, 0.0, 0.0},
        .acceleration = {0.0, 0.0, 0.0},
        
        .angularVelocity = {0.0, 0.0, 0.0},
        .angularAcceleration = {0.0, 0.0, 0.0},
        
        .csArea = 0.0079,
        .Cd = 0.8
    };

    // ======================
    // ENGINE
    // ======================

    Engine engine;
    engine.thrust = 600.0;          // N
    engine.Isp = 190.0;             // s
    engine.engineAngle = {deg_to_rad(0.0), deg_to_rad(0.0)};

    engine.geometryProperties.type = GeometryType::UniformCylinder;
    engine.geometryProperties.length = 0.18;      // m
    engine.geometryProperties.diameter = 0.090;   // m
    engine.geometryProperties.mass = 2.4;         // kg


    // ======================
    // NOSE
    // ======================

    Nose nose;
    nose.geometryProperties.type = GeometryType::SolidFrustum;
    nose.geometryProperties.length = 0.25;        // m
    nose.geometryProperties.diameter = 0.090;     // dla pełnego stożka
    nose.geometryProperties.mass = 0.35;          // kg


    // ======================
    // BODY TUBE
    // ======================

    BodyTube body;
    body.geometryProperties.type = GeometryType::HollowCylinder;
    body.geometryProperties.length = 0.95;        // m
    body.geometryProperties.mass = 1.10;          // kg
    body.geometryProperties.innerRadius = 0.043;  // m
    body.geometryProperties.outerRadius = 0.045;  // m


    // ======================
    // FUEL TANK
    // ======================

    FuelTank tank;
    tank.geometryProperties.type = GeometryType::UniformCylinder;
    tank.geometryProperties.length = 0.40;        // m
    tank.geometryProperties.diameter = 0.082;     // m
    tank.geometryProperties.mass = 3.20;          // kg


    // ======================
    // FINS
    // ======================

    Fin fin;
    fin.rootChord = 0.18;
    fin.tipChord = 0.08;
    fin.span = 0.09;
    fin.sweep = deg_to_rad(25.0);

    fin.geometryProperties.type = GeometryType::Trapezoid;
    fin.geometryProperties.length = 0.18;
    fin.geometryProperties.mass = 0.12;
    
    

    Rocket rocket;
    rocket.state = rocketState;
    rocket.structure.nose = nose;
    rocket.structure.fuelTank = tank;
    rocket.structure.engine = engine;
    rocket.structure.bodyTube = body;
    rocket.structure.fins.push_back(fin);
    rocket.structure.fins.push_back(fin);
    rocket.structure.fins.push_back(fin);
    

    rocket.initialize();

    
    PhysicsEngine physicsEngine;
    Logger logger("/Users/dawid/Desktop/Projekt Polaris/simulator/log.csv", rocket);

    double simuTime = 200.0; // seconds
    double t = 0.0;
    double dt = 0.05;

    double nextPrint = 0.0;
    const double printInterval = 0.05;

    double apogeum = 0;
    double apTime = 0;

    while(t <= simuTime) {
        logger.log(t);
        physicsEngine.update(rocket, t, dt); // dt = 0.05 seconds

        if(rocket.state.position.z() > apogeum) {
            apogeum = rocket.state.position.z();
            apTime = t;
        }

        // Rocket (hopefully) lands
        if(rocket.state.position.z() <= 0) {
            std::cout << "SIMULATION ENDED, REACHED GROUND" << std::endl;

            break;
        }

        t += dt;
    }

    std::cout << "Apogeum: " << apogeum << " at time: " << apTime << std::endl;

    return 0;
}