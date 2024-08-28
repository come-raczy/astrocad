#ifndef FOCUSER_CODES_H
#define FOCUSER_CODES_H

enum class CommandCode {
  FIRST_COMMAND = 0,  // avoid 0 to simplify 'toInt()' interpretation
  PING = 1,
  GET_INFO = 2,
  GET_VERSION = 3,
  GET_POSITION = 4,
  GET_DIRECTION = 5,
  GET_MAX_STEPS = 6,
  GET_REMAINING_STEPS = 7,
  GET_RPM = 8,
  GET_STEP_DURATION = 9,
  SET_RPM = 10,
  SET_ZERO = 11,
  MOVE = 12,
  MOVE_TO = 13,
  STOP = 14,
  LAST_COMMAND
};

enum class ResponseCode {
  FIRST_RESPONSE = 0,
  INTERNAL_ERROR = 1,
  OK = 2,
  NOK = 3,
  INVALID_COMMAND = 4,
  VERSION = 5,
  INFO = 6,
  POSITION = 7,
  DIRECTION = 8,
  MAX_STEPS = 9,
  REMAINING_STEPS = 10,
  RPM = 11,
  STEP_DURATION = 12,
  LAST_RESPONSE
};

// Separator between a code and the payload, of between fields of the payload
static const char SEPARATOR = ':';

/**
 ** message := CODE { PAYLOAD SEPARATOR }*
 ** CODE := CommandCode | ResponseCode
 **/

String toString(CommandCode const &code) {
  return String(static_cast<int>(code));
}

int toInt(CommandCode const &code) {
  return static_cast<int>(code);
}

String toString(ResponseCode const &code) {
  return String(static_cast<int>(code));
}
#endif  // #ifndef FOCUSER_CODES_H
