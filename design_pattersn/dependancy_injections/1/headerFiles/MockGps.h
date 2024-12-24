#pragma once
#include "IGps.h"

class MockGps: public IGPS
{
    // inherited via IGPS
    virtual float getLatitude() override;
    virtual float getLongitude() override;
};