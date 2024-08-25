#include <stdio.h>
#include "Command.h"
#include "Response.h"
#include "Motion.h"

const String INFO_STRING = "DIY focuser for Player One FHD-OAG Max, using 28BYJ-48 stepper motor";
const String VERSION_STRING = "v0.0.1";

// Mapping of motor controller pins to arduino pins
const unsigned int IN1 = 11;
const unsigned int IN2 = 10;
const unsigned int IN3 = 9;
const unsigned int IN4 = 8;

Motion motion;

//////////////////////////////////////////
// SETUP
//////////////////////////////////////////
void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ; // native USB requires waiting for the serial port
    }
    Serial.flush();
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    motion.initialize();
    // TODO: manage position in EEPROM
}

////////////////////////////////////////
// MAIN LOOP
////////////////////////////////////////
const unsigned int BUFFER_SIZE=32;
char buffer[BUFFER_SIZE];
Command command;
void loop() {
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
      return ping();
      break;
    case CommandCode::MOVE_TO:
      return ping();
      break;
    case CommandCode::STOP:
      return ping();
      break;
    default:
      return Response(ResponseCode::INVALID_COMMAND,toString(command.code()) + SEPARATOR + command.payload());
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

Response getRemainingSteps() {
  return Response(ResponseCode::REMAINING_STEPS, String(motion.remainingSteps()));
}

Response getRpm() {
  return Response(ResponseCode::RPM, String(motion.rpm()));
}

Response getStepDuration() {
  return Response(ResponseCode::RPM, String(motion.stepDuration()));
}

Response setRpm(const String &payload) {
  const unsigned long rpm = payload.toInt();
  if (0 == rpm) {
    return Response(ResponseCode::NOK, "Invalid RPM: " + payload);
  }
  motion.rpm(rpm);
  return Response(ResponseCode::OK, "RPM set successfully");
}

Response setZero() {
  motion.position(0);
  return Response(ResponseCode::OK, "Position succesfully set to 0");
}
