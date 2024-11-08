#pragma once
#include "main.h"
#include "math.h"
#pragma once


#include "pros/abstract_motor.hpp"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "pros/adi.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"

#ifndef PI
#define PI 3.14159265359
#endif

inline pros::Controller master(pros::E_CONTROLLER_MASTER);
inline pros::MotorGroup left({1,2});
inline pros::MotorGroup right({-3,-4});
inline pros::MotorGroup all({1,2,-3,-4});
inline pros::adi::DigitalOut piston('A');
inline pros::Imu inertial(7);


/*

Vex Constants

*/

// motor stuff
const double RPM_RED = 100;
const double RPM_GRN = 200;
const double RPM_BLU = 600;

const pros::MotorGears GEAR_RED = pros::MotorGears::red;
const pros::MotorGears GEAR_GREEN = pros::MotorGears::green;
const pros::MotorGears GEAR_BLUE = pros::MotorGears::blue;

const pros::MotorEncoderUnits ENCODER_ROTATIONS = pros::MotorEncoderUnits::rotations;
const pros::MotorEncoderUnits ENCODER_COUNTS = pros::MotorEncoderUnits::counts;
const pros::MotorEncoderUnits ENCODER_DEGREES = pros::MotorEncoderUnits::degrees;
const pros::MotorEncoderUnits ENCODER_INVALID = pros::MotorEncoderUnits::invalid;


const int MTR_MAX = 127;

// drivetrain ratio
const double DRIVE_RATIO = 0;

// wheel diameter + radius
const double WHL_DIAMETER = 2.75;
const double WHL_RADIUS = WHL_DIAMETER/2;

/*

Controller

*/

// get main controller
const inline pros::Controller MASTER(pros::E_CONTROLLER_MASTER);

// define a btn and stk variable
#define btn static constexpr pros::controller_digital_e_t
#define stk static constexpr pros::controller_analog_e_t

// map controller to variables
namespace controller {
    stk lx = pros::E_CONTROLLER_ANALOG_LEFT_X;
    stk ly = pros::E_CONTROLLER_ANALOG_LEFT_Y;

    stk rx = pros::E_CONTROLLER_ANALOG_RIGHT_X;
    stk ry = pros::E_CONTROLLER_ANALOG_RIGHT_Y;

    btn up = pros::E_CONTROLLER_DIGITAL_UP;
    btn down = pros::E_CONTROLLER_DIGITAL_DOWN;
    btn left = pros::E_CONTROLLER_DIGITAL_LEFT;
    btn right = pros::E_CONTROLLER_DIGITAL_RIGHT;

    btn A = pros::E_CONTROLLER_DIGITAL_A;
    btn B = pros::E_CONTROLLER_DIGITAL_B;
    btn X = pros::E_CONTROLLER_DIGITAL_X;
    btn Y = pros::E_CONTROLLER_DIGITAL_Y;

    btn lb = pros::E_CONTROLLER_DIGITAL_L1;
    btn lt = pros::E_CONTROLLER_DIGITAL_L2;

    btn rb = pros::E_CONTROLLER_DIGITAL_R1;
    btn rt = pros::E_CONTROLLER_DIGITAL_R2;
}

/*

Motors

*/

// motor ports (left)
const int L_FRNT_PORT = 1;
const int L_CNTR_PORT = 2;
const int L_REAR_PORT = 3;

// motor ports (right)
const int R_FRNT_PORT = 11;
const int R_CNTR_PORT = 12;
const int R_REAR_PORT = 13;

// motor ports (misc)
const int INTAKE_PORT = 10;

// Field Data
const double tile = 2;
const double field = 6*2;

// motors (left)
const inline pros::Motor LEFT_FRONT(L_FRNT_PORT, GEAR_BLUE, ENCODER_ROTATIONS);
const inline pros::Motor LEFT_CENTER(L_CNTR_PORT, GEAR_BLUE, ENCODER_ROTATIONS);
const inline pros::Motor LEFT_REAR(L_REAR_PORT, GEAR_BLUE, ENCODER_ROTATIONS);

#define L_REVERSE 0

#if L_REVERSE == 0
const inline pros::MotorGroup LEFT_GROUP({L_FRNT_PORT,L_CNTR_PORT,L_REAR_PORT}, GEAR_BLUE);
#elif L_REVERSE == 1
const inline pros::MotorGroup LEFT_GROUP({-L_FRNT_PORT,-L_CNTR_PORT,-L_REAR_PORT}, GEAR_BLUE);
#endif

// motors (right)
const inline pros::Motor RIGHT_FRONT(R_FRNT_PORT, GEAR_BLUE, ENCODER_ROTATIONS);
const inline pros::Motor RIGHT_CENTER(R_CNTR_PORT, GEAR_BLUE, ENCODER_ROTATIONS);
const inline pros::Motor RIGHT_REAR(R_REAR_PORT, GEAR_BLUE, ENCODER_ROTATIONS);

#define R_REVERSE 1

#if R_REVERSE == 0
const inline pros::MotorGroup RIGHT_GROUP({R_FRNT_PORT,R_CNTR_PORT,R_REAR_PORT}, GEAR_BLUE);
#elif R_REVERSE == 1
const inline pros::MotorGroup RIGHT_GROUP({-R_FRNT_PORT,-R_CNTR_PORT,-R_REAR_PORT}, GEAR_BLUE);
#endif

// motors (misc)
const inline pros::Motor INTAKE(INTAKE_PORT,GEAR_BLUE);

/*

ADI ports

*/

// pneumatics, sensors, etc.
const pros::adi::DigitalOut CLAW = pros::adi::DigitalOut(1);

/*

Methods and/or functions

*/

#define TIME_TYPE 1

#if TIME_TYPE == 0
inline double cur_time(){
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec+t.tv_nsec*0.000000001;
}
#elif TIME_TYPE == 1
inline double cur_time(){
    return pros::micros()*0.000001;
}
#endif

/*

Auton

*/

enum autonEnum {
    NOTHING = 0,
    DEFENSE = 1,
    OFFENSE = 2,

    SOLO_AWP_A = -1,
    SOLO_AWP_B = -2,
    SOLO_AWP_C = -3
};
inline int autonSelect = NOTHING;