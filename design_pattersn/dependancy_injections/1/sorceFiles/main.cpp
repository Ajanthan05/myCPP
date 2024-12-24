#include <iostream>
#include "IGps.h"
#include "Config.h"

void DriveRobot(IGPS &gps)
{
    float lat = gps.getLatitude();
    float lon = gps.getLongitude();
}
 
int main() 
{
    // // In interface class WE cannot create IGPS instant, we can create poinnters and reference
    // MockGps gps;
    // DriveRobot(gps);

    Config config;
    DriveRobot(config);
}