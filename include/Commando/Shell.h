#ifndef COMMANDO_SHELL_H
#define COMMANDO_SHELL_H

#include "CommandHandler.h"
#include "Commando/CommandHandler.h"
#include <functional>
#include <nonstd/string_view.hpp>
#include <string>
#include <utility>

namespace Commando {
struct Shell {
  static constexpr bool NO_ECHO = false;

  const char *leader      = "> ";
  const char *remove_char = "\b \b";

  std::string               buffer;
  CommandHandlerBase *const commands;

  const std::function<bool()>                    characterReady;
  const std::function<char()>                    getChar;
  const std::function<void(nonstd::string_view)> writeStr;

  const bool echoInput;

  Shell(CommandHandlerBase *const                commands,
        std::function<bool()>                    character_ready,
        std::function<char()>                    get_character,
        std::function<void(nonstd::string_view)> write_str,
        const bool                               echoInput = true):
      commands(commands),
      characterReady(std::move(character_ready)),
      getChar(std::move(get_character)),
      writeStr(std::move(write_str)),
      echoInput(echoInput) {}

  void exec() {
    commands->handle_input(buffer);
    buffer.clear();
    writeStr("\n");
    writeStr(leader);
  }

  void step() {
    if(!characterReady()) {
      return;
    }
    const char in = getChar();
    if(in == '\n') {
      exec();
    } else if(in == '\b' /*backslash*/) {
      buffer.pop_back();
      writeStr(remove_char);
    } else {
      buffer.push_back(in);
      if(echoInput != NO_ECHO) {
        writeStr(nonstd::string_view(&in, /*count=*/1));
      }
    }
  }
};
}    // namespace Commando

#endif
