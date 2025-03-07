#include "vehicle_factory.h"

Vehicle* VehicleFactory::getVehicle(string vehicleType) {
    Vehicle *vehicle;

    if (vehicleType == "Car") {
        vehicle = new Car();
    }
    else if (vehicleType == "Bike") {
        vehicle = new Bike();
    }
    // vehicle->createVehicle(); my mistake
    
    return vehicle;
}

// g++ -c vehicle_factory.cpp vehicle.cpp car.cpp bike.cpp 
// ar ru vehicle_library.a car.o bike.o vehicle_factory.o
// g++ -o client smart_clint.cpp vehicle_library.a 