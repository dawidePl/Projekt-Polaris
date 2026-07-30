#include "Logger.h"

void Logger::log(double t) {
    this->logFile   << t << ","
                    << this->rocket.state.position.x() << ", "
                    << this->rocket.state.position.y() << ", "
                    << this->rocket.state.position.z() << ", "

                    << this->rocket.state.rotation.x() << ", "
                    << this->rocket.state.rotation.y() << ", "
                    << this->rocket.state.rotation.z() << ", "
                    
                    << this->rocket.state.velocity.x() << ", "
                    << this->rocket.state.velocity.y() << ", "
                    << this->rocket.state.velocity.z() << ", "

                    << this->rocket.state.acceleration.x() << ", "
                    << this->rocket.state.acceleration.y() << ", "
                    << this->rocket.state.acceleration.z() << ", "

                    << this->rocket.structure.fuelTank.geometryProperties.mass << ", "
                    << this->rocket.getMass() << ", "

                    << this->rocket.state.csArea << ", "
                    << this->rocket.state.Cd << ", "

                    << this->rocket.structure.engine.thrust << ", "
                    << this->rocket.getThrust(t).x() << ", "
                    << this->rocket.getThrust(t).y() << ", "
                    << this->rocket.getThrust(t).z() << ", "
                    << this->rocket.structure.engine.Isp << ", "
                    << this->rocket.structure.engine.engineAngle.x() << ", "
                    << this->rocket.structure.engine.engineAngle.y() << std::endl;
}