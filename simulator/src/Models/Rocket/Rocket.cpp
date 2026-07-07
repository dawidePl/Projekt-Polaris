#include "Rocket.h"

// vec3<double> RocketEngine::getThrust() {
//     return {0.0, 0.0, 100.0};
// }

vec3<double> RocketEngine::getThrust() {
    mat3<double> R = mat3<double>::rotation_y(this->engineAngle.y()) * mat3<double>::rotation_x(this->engineAngle.x());
    vec3<double> Ft = {0.0, 0.0, 100.0}; // N, TODO: implement based on Ft(t)

    return R * Ft;
}