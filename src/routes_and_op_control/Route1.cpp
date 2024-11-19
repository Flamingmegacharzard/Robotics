#include "Route1.hpp"
#include "..\globals.hpp"
#include "lib/autonlibrary.hpp"
#include "pros/rtos.hpp"

void route1(){
    all.set_encoder_units_all(ENCODER_DEGREES);
    pros::delay(1000);
    turn(1000000, 300, 1, 1, 0, false);
}