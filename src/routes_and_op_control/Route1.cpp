#include "Route1.hpp"
#include "..\globals.hpp"
#include "lib/autonlibrary.hpp"
#include "pros/rtos.hpp"

void route1(){
    all.set_encoder_units_all(ENCODER_ROTATIONS);
    pros::delay(1000);
    advance(1, 600);
}