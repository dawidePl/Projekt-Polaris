#pragma once

#include <cmath>

namespace Atmosphere {
    double T0 = 288.16; // Kelvin
    double alfa = 0.0065; // K/m temp change per meter
    double P0 = 101325; // Pressure at h=0, Pascals
    double rho0 = 1.225; // Density at h=0, Kg/m^3
    double n = 5.2561; // idk, for calculations
    
    double getTemperature(double h);
    double getPressure(double h);
    double getDensity(double h);
};