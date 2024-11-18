#include "Route1.hpp"
#include "..\globals.hpp"
#include "lib/autonlibrary.hpp"
#include "pros/rtos.hpp"

void route1(){
    all.set_encoder_units_all(ENCODER_DEGREES);
    pros::delay(1000);
    advance(44, 1, 1, 300, 0);
}