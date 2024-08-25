#ifndef FOCUSER_COMMAND_H
#define FOCUSER_COMMAND_H

#include "codes.h"
class Command {
public:
    int decode() {
        const int separator = buffer.indexOf(SEPARATOR);
        if (separator <= 0) {
            return -1;
        }
        const int tmp = buffer.substring(0, separator).toInt();
        static const int first = toInt(CommandCode::FIRST_COMMAND);
        static const int last = toInt(CommandCode::LAST_COMMAND);
        if ((tmp <= first) || (tmp >= last)) {
            return -1;
        }
        code_ = static_cast<CommandCode>(buffer.substring(0, separator).toInt());
        payload_ = buffer.substring(separator + 1);
        return 0;
    }
    CommandCode code() const {return code_;}
    const String &payload() const {return payload_;}
    String buffer;
protected:
    CommandCode code_;
    String payload_;
private:
};

#endif // #ifndef FOCUSER_COMMAND_H
