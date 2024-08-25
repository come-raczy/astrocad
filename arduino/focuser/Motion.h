#ifndef FOCUSER_MOTION_H
#define FOCUSER_MOTION_H

#include "Command.h"
#include "Response.h"
#include <stdlib.h>

// Firmware for a focuser based on the 28BYJ-48 motor.
// The motor has 32 steps per revolution and a gearbox with a ration of 64:1,
// for a total of 2048 steps per revolution. The pinion has 18 teeth. The gear
// has 56 teeths, for an extra 56/18:1 gear ratio.
//  The focuser is the helical focuser from the Player One FHD-OAG MAX. It has a
//  maximum rotation of 9/10 of a turn.
// The total number of steps is : 2048 * 56 * 9 / 18 / 10 ~= 5734

const unsigned long BASE_STEPS = 32;
const unsigned long GEARBOX = 64;
const unsigned long PINION_TEETH = 18;
const unsigned long GEAR_TEETH = 56;
const float MAX_ROTATION = 9.0 / 10.0;

class Motion {
public:
  // number of steps for 1 full revolution of the focuser ring
  static unsigned long stepsPerRev() {
    return double(BASE_STEPS * GEARBOX * GEAR_TEETH) / double(GEAR_TEETH);
  }
  // total number of steps for the full range of motion of the focuser ring
  static const unsigned long maxSteps() { return stepsPerRev() * MAX_ROTATION; }
  static const unsigned int DEFAULT_RPM = 3;
  // step duration in microseconds to reach rhe targeted default RPM
  static const unsigned long defaultStepDuration() {
    return 60 * 1000L * 1000L / maxSteps() / DEFAULT_RPM;
  }

  enum Direction { FORWARD = 1, BACKWARD = -1, IDLE = 0 };
  void initialize() {
    // TODO read position from EEPROM
    position_ = 0;
    direction_ = IDLE;
    destination_ = 0;
  }
  void setPosition(unsigned int position) {
    position_ = position;
    stop();
  }
  void stop() {
    direction_ = IDLE;
    destination_ = position_;
    // TODO: save position to EEPROM
  }
  unsigned int position() const { return position_; }
  void position(const int position) {position_ = position;}
  Direction direction() const { return direction_; }
  unsigned int remainingSteps() const {
    return abs(int(destination_) - int(position_));
  }
  unsigned int rpm() const {return rpm_;}
  unsigned long stepDuration() const {return stepDuration_;}
  void rpm(const unsigned int rpm) {
    rpm_ = rpm;
    stepDuration_ = 60 * 1000L * 1000L / maxSteps() / rpm_;
  }
protected:
  Direction direction_ = IDLE;
  unsigned int position_ = 0;
  unsigned int destination_ = 0;
  unsigned long stepTime_ = 0;
  unsigned int rpm_;
  unsigned long stepDuration_;
};

#endif // #ifndef FOCUSER_MOTION_H
