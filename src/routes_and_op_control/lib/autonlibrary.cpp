#include "../../globals.hpp"
#include "autonlibrary.hpp"

double GEAR_RATIO = 0.80;
int TIMEOUT = 5000;

/* Method Description:
Since get_position_all() returns a vector, we have to split up the motor
group and add the positions of all the motors. After they've all been aded
up, we will divide the total by the number of motors to find the average and
return it.

@returns double total / (number_of_left_motors + number_of_right_motors)
*/
double avg_encoder()
{
  double total = 0.0;

  // Gets number of motors for each motor group
  int number_of_left_motors = left.get_position_all().size();
  int number_of_right_motors = right.get_position_all().size();

  // Adds the position of each motor within the left motor group to total
  for (int i = 0; i < number_of_left_motors-1; i++) {
    total += left.get_position(i);
  }
  // Adds the position of each motor within the right motor group to total
  for (int i = 0; i < number_of_right_motors-1; i++){
    total += right.get_position(i);
  }

  // Adds the number of motors there are and divides the tatal with the total
  // number of motors. Returns it afterwards.
  return total / (number_of_left_motors + number_of_right_motors);
}

double previous_error = 0;

double PD(double kp, double kd, double target_dist, double current_position){
  double error = target_dist - current_position;
  double tmp_p = error * kp;
  double tmp_d = (error - previous_error) * kd;
  previous_error = error;
  return tmp_p + tmp_d;
}

/* Method Description:
Since the robot will turn more than 180 degrees to get to the same angle when
turned less than -180 degrees, this method will take a degree, find the fastest
route to that angle, and return a faster degree that will get you to that same
angle.


@param double target_heading
You enter the degree you want to head to here.

@return double positionaldifference
*/
double find_nearest_heading(double target_heading) {
  // Finds the heading entered to the nearest 360 degrees. (bounds the heading)
  double position = std::fmod(target_heading, 360);

  // Finds the difference between the desired heading and the heading the robot
  // is at the moment in.
  double positiondifference = position - inertial.get_heading();

  // If the difference is more than 180 (meaning you'll need to turn more than
  // 180), subtract 360 to head the other way because it's shorter.
  if (positiondifference > 180) {
    positiondifference = positiondifference - 360;
  }

  // If the difference is more than -180 (meaning you'll need to turn more than
  // -180), add 360 to head the other way because it's shorter.
  if (positiondifference < -180) {
    positiondifference = positiondifference + 360;
  }

  // Returns the determined best heading
  return positiondifference;
}

// Ewwww time based (don't use unless you want to)
void time_forward(int voltage, int time) {
  int move_sec = time * 1000;
  all.move(voltage);
  pros::delay(move_sec);
  all.move(0);
}

// Nasty ass time based turn code ew
void time_turn(int voltage, int time) {
  int move_sec = time * 1000;
  left.move(voltage);
  right.move(-voltage);
  pros::delay(move_sec);
  all.move(0);
}

/* Method Description:
This is a improved autonomous code I made (PID not yet fully implemented, that's
your job). Allows your robot to move backwards and forwards on the feild.

@params double pos, int velocity
Enter a position and velocity into the parameters.
*/
void advance(double inches, double kP, double kD, double max_velocity, double decel_zone) {

  // Resets all motor's position values.
  all.tare_position_all();

  // Calculates the distance needed to travel in inches.
  double dist = (inches / (WHL_DIAMETER * PI)) * 360 * GEAR_RATIO;

  double velocity = max_velocity;

  // Calculates the exit time for the route incase the route takes too long.
  double exit_time = pros::c::millis() + TIMEOUT;

  // While the code is below the exit time and more below the targeted distance.
  while (exit_time > pros::c::millis()){

    if (((avg_encoder() >= (dist * decel_zone)) && (dist > 0)) || ((avg_encoder() <= (dist * decel_zone)) && (dist < 0))){
      velocity = PD(kP, kD, dist, avg_encoder());
    }

    if (((dist - 1) <= avg_encoder()) && (avg_encoder() <= (dist + 1))){
      break;
    }

    // All motors must move towards this distance at a certain velocity.

    master.print(1, 0, "%f", avg_encoder());

    all.move_velocity(velocity);

    if (velocity > max_velocity){
      velocity = max_velocity;
    }

    // If the robot is more than 80% to way to the distance, slow down the robot
    // by dividing the velocity by 5% over each time.

    // Delays for 10 milliseconds to save memory.
    pros::delay(10);
  }
  
  // Makes sure the velocity stops
  all.move_velocity(0);
  pros::delay(10);
  double previous_error = 0;
}

/* Method Description:
Improved autonomous turn code allows your robot to turn to a heading you assign
it. Will turn both clockwise and counterclockwise to the assigned angle in the
fastest heading. (PID not fully implemented)

@params double heading, int velocity, bool do_not_auto_set_to_best_angle
Enter the desired heading, the velocity, and if you want the code to find the
best heading to turn to.
*/
void turn(double heading, double max_velocity, double kP, double kD, double decel_zone, bool do_not_auto_set_to_best_angle) {
  // Resets imu
  inertial.reset();
  double targeted_heading = find_nearest_heading(heading);

  double velocity = max_velocity;

  // If you don't want the code to automatically set it to the best angle, the
  // targeted heading will be equal to heading.
  if (do_not_auto_set_to_best_angle == true) {
    targeted_heading = heading;
  }


  // If the targeted heading is negative, reverse the velocity/motors
  if (targeted_heading < 0) {
    velocity = -velocity;
  }

  // Determines amount of time before exiting the code.
  double exit_time = pros::c::millis() + TIMEOUT;

  // If the exit time is not reached or the destination is not reached, loop the
  // code.
  while (exit_time > pros::c::millis()) {
    // Rotates on the pivot right(default). It would be left if the targeted
    // heading is negative.
    master.print(1, 0, "%d", inertial.get_heading());
    left.move_velocity(velocity);
    right.move_velocity(-velocity);

    if (((inertial.get_heading() >= (targeted_heading*decel_zone)) && (targeted_heading > 0)) || (((inertial.get_heading() <= (targeted_heading*decel_zone))) && (targeted_heading < 0))){
      velocity = PD(kP, kD, targeted_heading, inertial.get_heading());
    }

    if (((targeted_heading - 1) <= inertial.get_heading()) && (inertial.get_heading() <= (targeted_heading + 1))){
      break;
    }

    if (velocity > max_velocity){
      velocity = max_velocity;
    }

    // Delays for 10 milliseconds to save memory.
    pros::delay(10);
  }

  // Makes sure the velocity stops
  all.move_velocity(0);
  pros::delay(10);
}