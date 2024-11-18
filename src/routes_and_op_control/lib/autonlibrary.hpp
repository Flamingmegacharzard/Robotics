#pragma once

void time_forward(int voltage, int time);
void time_turn(int voltage, int time);
void advance(double inches, double kP, double kD, double max_velocity, double decel_zone);
void turn(double heading, int max_velocity, double kP, double kD, double decel_zone, bool do_not_auto_set_to_best_angle);