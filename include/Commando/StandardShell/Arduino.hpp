#include <Arduino.h>
#include <Commando/CommandHandler.h>
#include <Commando/Shell.h>
#include <iostream>

using namespace Commando;
struct ArduinoShell : public Shell {
  ArduinoShell(CommandHandlerBase &command_handler, const Stream &stream = Serial):
      Shell(
        command_handler,
        [&]() { return stream.available() > 0; },
        [&]() { return stream.read(); },
        [&](nonstd::string_view in) { stream.write(in.begin(), in.size()); }){};
};
