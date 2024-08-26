#ifndef FOCUSER_CODES_H
#define FOCUSER_CODES_H

enum class CommandCode {
  FIRST_COMMAND = 0,  // avoid 0 to simplify 'toInt()' interpretation
  PING = 1,
  GET_INFO,
  GET_VERSION,
  GET_POSITION,
  GET_DIRECTION,
  GET_MAX_STEPS,
  GET_REMAINING_STEPS,
  GET_RPM,
  GET_STEP_DURATION,
  SET_RPM,
  SET_ZERO,
  MOVE,
  MOVE_TO,
  STOP,
  LAST_COMMAND
};

enum class ResponseCode {
  FIRST_RESPONSE = 0,
  INTERNAL_ERROR,
  OK,
  NOK,
  INVALID_COMMAND,
  VERSION,
  INFO,
  POSITION,
  DIRECTION,
  MAX_STEPS,
  REMAINING_STEPS,
  RPM,
  STEP_DURATION,
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
