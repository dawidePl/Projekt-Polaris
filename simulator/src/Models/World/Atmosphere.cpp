#include "Atmosphere.h"

double Atmosphere::getTemperature(double h) {
    return Atmosphere::T0 - Atmosphere::alfa * h;
}

double Atmosphere::getPressure(double h) {
    return std::pow((Atmosphere::getTemperature(h) * Atmosphere::P0) / Atmosphere::T0, Atmosphere::n);
}

double Atmosphere::getDensity(double h) {
    return Atmosphere::rho0 * std::pow((Atmosphere::getTemperature(h) / Atmosphere::T0), Atmosphere::n - 1);
}