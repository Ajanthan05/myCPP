#pragma once
#include "IGps.h"
#include "MockGps.h"
#include "RealGps.h"

class Config
{
private:
    MockGps gps;
public:
    IGPS& getGPS();
};