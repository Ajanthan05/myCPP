#ifndef VEHICLE_FACTORY_H
#define VEHICLE_FACTORY_H

#include <iostream>
#include "car.h"
#include "bike.h"
using namespace std;

class VehicleFactory {
public:
    static Vehicle *getVehicle(string vehicleType); // Static boc access the function without the object
};

#endif