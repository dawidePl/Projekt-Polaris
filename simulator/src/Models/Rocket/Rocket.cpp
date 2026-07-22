#include "Rocket.h"

double Rocket::getBurnTime() {
    double mdot = this->engine.thrust / (this->engine.Isp * 9.80665);

    return this->state.massFuel / mdot;
}

// TODO: loading thrust data from OpenRocket data
vec3<double> Rocket::getThrust(double t) {
    mat3<double> R = mat3<double>::rotation_y(this->engine.engineAngle.y()) * mat3<double>::rotation_x(this->engine.engineAngle.x());
    
    double bt = this->getBurnTime();
    vec3<double> Ft = {0.0, 0.0, this->engine.thrust};

    if(bt < t)
        Ft = {0.0, 0.0, 0.0};

    return R * Ft;
}
