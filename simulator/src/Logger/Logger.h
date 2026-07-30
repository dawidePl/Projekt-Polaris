#pragma once

#include <fstream>

#include "../Models/Rocket/Rocket.h"

class Logger {
private:
    std::fstream logFile;
    Rocket &rocket;
public:
    Logger(std::string logFilePath, Rocket &rocket) : rocket(rocket) {
        this->logFile.open(logFilePath, std::ios::out);
        
        this->logFile << "Time,PositionX,PositionY,PositionZ,RotationX,RotationY,RotationZ,VelocityX,VelocityY,VelocityZ,AccelerationX,AccelerationY,AccelerationZ,MassFuel,MassStructural,CsArea,Cd,Thrust,ForceThrustX,ForceThrustY,ForceThrustZ,Isp,EngineAngleX,EngineAngleY" << std::endl;
    }

    ~Logger() {
        this->logFile.close();
    }

    void log(double t);
};