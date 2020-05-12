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

  std::string         buffer;
  CommandHandlerBase &commands;

  const std::function<bool()>                    character_ready;
  const std::function<char()>                    get_char;
  const std::function<void(nonstd::string_view)> write_str;

  const bool echo_input;

  Shell(CommandHandlerBase &                     commands,
        std::function<bool()>                    character_ready,
        std::function<char()>                    get_character,
        std::function<void(nonstd::string_view)> write_str,
        const bool                               echo_input = true):
      commands(commands),
      character_ready(std::move(character_ready)),
      get_char(std::move(get_character)),
      write_str(std::move(write_str)),
      echo_input(echo_input) {}

  void exec() {
    commands.handle_input(buffer);
    buffer.clear();
    write_str("\n");
    write_str(leader);
  }

  void step() {
    if(!character_ready()) {
      return;
    }
    const char in = get_char();
    if(in == '\n') {
      exec();
    } else if(in == '\b' /*backslash*/) {
      buffer.pop_back();
      write_str(remove_char);
    } else {
      buffer.push_back(in);
      if(echo_input != NO_ECHO) write_str(nonstd::string_view(&in, 1));
    }
  }
};
}    // namespace Commando

#endif
