#pragma once

void time_forward(int voltage, int time);
void time_turn(int voltage, int time);
void advance(double inches, double velocity);
void turn(double heading, int velocity, bool do_not_auto_set_to_base_angle);