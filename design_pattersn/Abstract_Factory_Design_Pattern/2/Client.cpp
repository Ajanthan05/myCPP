#ifndef __io__
#define __io__

#include <iostream>
using namespace std;
#endif

#include "CarFactory.cpp"

#define SIMPLE_CAR 1

int main() {
    #ifdef SIMPLE_CAR
        CarFactory* factory = new SimpleCarFactory;
    
    #elif LUXUARY_CAR
        CarFactory* factory = new LuxuryCarFactory;

    #endif

    Car *car = factory->buildWholeCar();
    car->printDetails();

    return 0;
}