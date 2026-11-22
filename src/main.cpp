/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       kavin                                                     */
/*    Created:      8/22/2026, 2:15:47 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// A global instance of the controller
controller Controller1 = controller(primary);

// Drivetrain motors — ADJUST PORTS to match your robot's wiring.
// Assumes a standard 4-motor tank drive: 2 motors per side.
motor LeftFront  = motor(PORT19, ratio18_1, true);
motor LeftBack   = motor(PORT17, ratio18_1, true);
motor RightFront = motor(PORT20, ratio18_1, false); 
motor RightBack  = motor(PORT18, ratio18_1, false); 

motor_group LeftDrive  = motor_group(LeftFront, LeftBack);
motor_group RightDrive = motor_group(RightFront, RightBack);

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Function                         */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Reset encoders and set motors to hold/brake as needed.
  LeftDrive.setStopping(brake);
  RightDrive.setStopping(brake);
  LeftDrive.resetPosition();
  RightDrive.resetPosition();
}

/*---------------------------------------------------------------------------*/
/*                              Autonomous Task                              */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // Simple example autonomous: drive forward, stop, run intake.
  LeftDrive.spin(forward, 50, percent);
  RightDrive.spin(forward, 50, percent);
  wait(1500, msec);
  
  LeftDrive.spin(forward, 25, percent);
  RightDrive.spin(forward, 25, percent);
  wait(1500, msec);
  LeftDrive.stop();
  RightDrive.stop();
}

/*---------------------------------------------------------------------------*/
/*                              User Control Task                            */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // Deadband to avoid drift from joystick noise near center.
  const int deadband = 5;

  while (1) {
    int throttle = Controller1.Axis3.position(); // left stick, forward/back
    int turn     = Controller1.Axis1.position(); // right stick, left/right

    if (abs(throttle) < deadband) throttle = 0;
    if (abs(turn) < deadband) turn = 0;

    int leftSpeed  = throttle + turn;
    int rightSpeed = throttle - turn;

    // Clamp to valid percent range.
    leftSpeed  = leftSpeed > 100 ? 100 : (leftSpeed < -100 ? -100 : leftSpeed);
    rightSpeed = rightSpeed > 100 ? 100 : (rightSpeed < -100 ? -100 : rightSpeed);

    LeftDrive.spin(forward, leftSpeed, percent);
    RightDrive.spin(forward, rightSpeed, percent);

    wait(20, msec); // Sleep the task for a short amount of time to
                     // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Run the pre-autonomous function.
  pre_auton();

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}