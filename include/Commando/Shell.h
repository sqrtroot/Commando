#ifndef COMMANDO_SHELL_H
#define COMMANDO_SHELL_H

#include "CommandHandler.h"
#include "Commando/CommandHandler.h"
#include <functional>
#include <nonstd/string_view.hpp>
#include <utility>
#include <string>

namespace Commando {
struct Shell {
  const char *                             leader = "> ";
  std::string                              buffer;
  CommandHandlerBase &                     commands;
  const std::function<bool()>                    character_ready;
  const std::function<char()>                    get_char;
  const std::function<void(nonstd::string_view)> write_str;

  Shell(CommandHandlerBase &               commands,
        std::function<bool()>                    character_ready,
        std::function<char()>                    get_character,
        std::function<void(nonstd::string_view)> write_str):
      commands(commands),
      character_ready(std::move(character_ready)),
      get_char(std::move(get_character)),
      write_str(std::move(write_str)) {}

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
    }else{
      buffer.push_back(in);
    }
  }
};
}    // namespace Commando

#endif
