#pragma once

class IGPS
{
public:
    virtual float getLatitude() = 0;
    virtual float getLongitude() = 0;
};