#pragma once

#include "../Rocket/Rocket.h"
#include "Atmosphere.h"

struct World {
    double simulationTime;
    
    // Todo: Atmosphere and virtually any part of the world that will be simulated
    Rocket rocket;
};