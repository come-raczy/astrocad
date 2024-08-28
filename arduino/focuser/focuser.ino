#include <stdio.h>
//#include <initializer_list>
#include "Command.h"
#include "Response.h"
#include "Motion.h"

const String INFO_STRING = "DIY focuser for Player One FHD-OAG Max, using 28BYJ-48 stepper motor";
const String VERSION_STRING = "v0.0.1";


// Mapping of motor controller pins to arduino pins
const unsigned int IN_PINS[4] = { 7, 8, 9, 10 };


Motion motion(IN_PINS);

//////////////////////////////////////////
// SETUP
//////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;  // native USB requires waiting for the serial port
  }
  Serial.flush();
  motion.initialize();
  // TODO: manage position in EEPROM
  int l[] = { 11, 12, 10, 9, 8, 7, 6, 5, 4, 3, 2 };
  for (auto p : l) {
    pinMode(p, OUTPUT);
    digitalWrite(p, LOW);
  }
  digitalWrite(12, HIGH);
}

////////////////////////////////////////
// MAIN LOOP
////////////////////////////////////////
const unsigned int BUFFER_SIZE = 32;
char buffer[BUFFER_SIZE];
Command command;
void loop() {
  // check for incoming commands
  if (Serial.available() > 0) {
    command.buffer = Serial.readStringUntil('\n');
    if (command.decode() != 0) {
      Response response(ResponseCode::INVALID_COMMAND, command.buffer);
      Serial.println(response.encode().c_str());
    } else {
      const Response response = execute(command);
      Serial.println(response.encode().c_str());
    }
  }
  // execute the actual motion, if any
  motion.step();
}

Response execute(const Command &command) {
  switch (command.code()) {
    case CommandCode::PING:
      return ping();
      break;
    case CommandCode::GET_INFO:
      return getInfo();
      break;
    case CommandCode::GET_VERSION:
      return getVersion();
      break;
    case CommandCode::GET_POSITION:
      return getPosition();
      break;
    case CommandCode::GET_DIRECTION:
      return getDirection();
      break;
    case CommandCode::GET_MAX_STEPS:
      return getMaxSteps();
      break;
    case CommandCode::GET_REMAINING_STEPS:
      return getRemainingSteps();
      break;
    case CommandCode::GET_RPM:
      return getRpm();
      break;
    case CommandCode::GET_STEP_DURATION:
      return getStepDuration();
      break;
    case CommandCode::SET_RPM:
      return setRpm(command.payload());
      break;
    case CommandCode::SET_ZERO:
      return setZero();
      break;
    case CommandCode::MOVE:
      return move(command.payload());
      break;
    case CommandCode::MOVE_TO:
      return moveTo(command.payload());
      break;
    case CommandCode::STOP:
      return stop();
      break;
    default:
      return Response(ResponseCode::INVALID_COMMAND, toString(command.code()) + SEPARATOR + command.payload());
      break;
  }
}

Response ping() {
  return Response(ResponseCode::OK, "");
}

Response getInfo() {
  return Response(ResponseCode::INFO, INFO_STRING);
}

Response getVersion() {
  return Response(ResponseCode::VERSION, VERSION_STRING);
}

Response getPosition() {
  return Response(ResponseCode::POSITION, String(motion.position()));
}


Response getDirection() {
  return Response(ResponseCode::DIRECTION, String(motion.direction()));
}

Response getMaxSteps() {
  return Response(ResponseCode::MAX_STEPS, String(motion.maxSteps()));
}

Response getRemainingSteps() {
  return Response(ResponseCode::REMAINING_STEPS, String(motion.remainingSteps()));
}

Response getRpm() {
  return Response(ResponseCode::RPM, String(motion.rpm()));
}

Response getStepDuration() {
  return Response(ResponseCode::STEP_DURATION, String(motion.stepDuration()));
}

Response setRpm(const String &payload) {
  const unsigned long rpm = payload.toInt();
  if (0 == rpm) {
    return Response(ResponseCode::NOK, "Invalid RPM: " + payload);
  }
  motion.rpm(rpm);
  return Response(ResponseCode::OK, "RPM set");
}

Response setZero() {
  motion.position(0);
  return Response(ResponseCode::OK, "Position succesfully set to 0");
}

Response move(const String &payload) {
  const long steps = payload.toInt();
  if (0 == steps) {
    return Response(ResponseCode::NOK, "Invalid number of steps: " + payload);
  }
  const int res = motion.move(steps);
  if (res != 0) {
    return Response(ResponseCode::NOK, "Can't move " + payload + " steps");
  }
  return Response(ResponseCode::OK, "move");
}

Response moveTo(const String &payload) {
  const long position = payload.toInt();
  if ((0 <= position) && (payload != "0")) {
    return Response(ResponseCode::NOK, "Invalid position: " + payload);
  }
  const int res = motion.moveTo(position);
  if (res != 0) {
    return Response(ResponseCode::NOK, "Can't move to position " + payload);
  }
  return Response(ResponseCode::OK, "moveTo");
}

Response stop() {
  motion.stop();
  return Response(ResponseCode::OK, "stop");
}