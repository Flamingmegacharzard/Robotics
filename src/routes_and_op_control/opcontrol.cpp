#include "opcontrol.hpp"
#include "../globals.hpp"
void opcontrol_loop(){
    while (true) {
    // Finds left joystick's Y axis.
    int move_y = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);

    // Finds right joystick's X axis.
    int move_x = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

    // Moves the motors based on the controller's values.
    left.move(move_y + move_x);
    right.move(move_y - move_x);

    // Delays to save memory.
    pros::delay(10);
  }
}