#ifndef FOCUSER_RESPONSE_H
#define FOCUSER_RESPONSE_H

#include "codes.h"
class Response {
public:
  Response(ResponseCode code, const String &payload)
    : code(code), payload(payload) {}
  String encode() {
    if ((code <= ResponseCode::FIRST_RESPONSE) || (code >= ResponseCode::LAST_RESPONSE)) {
      payload += String(SEPARATOR) + String("Invalid ResponseCode") + String(SEPARATOR) + toString(code);
      code = ResponseCode::INTERNAL_ERROR;
    }
    return toString(code) + String(SEPARATOR) + payload;
  }
  ResponseCode code;
  String payload;
protected:
private:
};

#endif  // #ifndef FOCUSER_RESPONSE_H
