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
  Motion(const unsigned int inPins[4])
    : inPins_{ inPins } {}
  // number of steps for 1 full revolution of the focuser ring
  static unsigned long stepsPerRev() {
    return double(BASE_STEPS * GEARBOX * GEAR_TEETH) / double(GEAR_TEETH);
  }
  // total number of steps for the full range of motion of the focuser ring
  static const unsigned long maxSteps() {
    return stepsPerRev() * MAX_ROTATION;
  }
  static const unsigned int DEFAULT_RPM = 3;
  // step duration in microseconds to reach rhe targeted default RPM
  static const unsigned long defaultStepDuration() {
    return 60 * 1000L * 1000L / maxSteps() / DEFAULT_RPM;
  }
  void initialize() {
    for (auto p : inPins_) {
      pinMode(p, OUTPUT);
    }
    // TODO read position from EEPROM
    position_ = 0;
    destination_ = 0;
    lastStepTime_ = 0;
    ;
  }
  void setPosition(unsigned int position) {
    position_ = position;
    stop();
  }
  void stop() {
    destination_ = position_;
    // TODO: save position to EEPROM
    for (auto p : inPins_) {
      // And de-energize the stepper by setting all the pins to LOW to save power,
      // prevent heat build up, and eliminate vibrations.
      digitalWrite(p, LOW);
    }
  }
  unsigned int position() const {
    return position_;
  }
  void position(const int position) {
    position_ = position;
  }
  int direction() const {
    return position_ < destination_ ? 1 : (position_ > destination_ ? -1 : 0);
  }
  unsigned int remainingSteps() const {
    return abs(int(destination_) - int(position_));
  }
  unsigned int rpm() const {
    return rpm_;
  }
  unsigned long stepDuration() const {
    return stepDuration_;
  }
  void rpm(const unsigned int rpm) {
    rpm_ = rpm;
    stepDuration_ = 60 * 1000L * 1000L / maxSteps() / rpm_;
  }
  int move(const int steps) {
    long destination = int(position()) + steps;
    if (destination < 0) {
      return -1;
    }
    return moveTo(destination);
  }
  int moveTo(const unsigned int destination) {
    if (destination > maxSteps()) {
      return -1;
    }
    destination_ = destination;
    return 0;
  }
  void step() {
    if (position_ == destination_) {
      return;
    }
    unsigned long now = micros();
    if (now < lastStepTime_ + stepDuration()) {
      return;
    }
    lastStepTime_ = now;
    if (position_ < destination_) {
      position_++;
    } else {
      position_--;
    }
    unsigned pinLevels = pinLevels_[position_ % 8];
    for (unsigned int i = 0; i < 4; ++i) {
      digitalWrite(inPins_[i], pinLevels & 1);
      pinLevels >> 1;
    }
    if (position_ == destination_) {
      stop();
    }
  }
protected:
  const unsigned int inPins_[4];
  unsigned int position_ = 0;
  unsigned int destination_ = 0;
  unsigned long lastStepTime_ = 0;
  unsigned int rpm_;
  unsigned long stepDuration_;
  const char pinLevels_[8] = { 0b1001, 0b1000, 0b1100, 0b0100, 0b0110, 0b0010, 0b0011, 0b0001 };
};

#endif  // #ifndef FOCUSER_MOTION_H
