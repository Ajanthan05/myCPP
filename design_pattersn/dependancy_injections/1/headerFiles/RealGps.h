#pragma once
#include "IGps.h"

class RealGps: public IGPS
{
    // inherited via IGPS
    virtual float getLatitude() override;
    virtual float getLongitude() override;
};