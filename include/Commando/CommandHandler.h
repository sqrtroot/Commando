/** Copyright 2020 Robert "sqrtroot" Bezem
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
       http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/
#ifndef COMMANDO_COMMANDHANDLER_H
#define COMMANDO_COMMANDHANDLER_H

#include "ArgSpan.h"
#include "Command.h"
#include "CommandStatus.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <nonstd/string_view.hpp>
#include <type_traits>
#include <vector>

namespace Commando {
struct CommandHandlerBase {
  static std::vector<nonstd::string_view> parse_arguments(nonstd::string_view input);
  virtual CommandStatus handle_input(const nonstd::string_view &input) = 0;
};

template<size_t C>
class CommandHandler : public CommandHandlerBase {
public:
  const std::array<Command *, C> commands;

  explicit CommandHandler(const std::array<Command *, C> &commands):
      commands(commands){};
  explicit CommandHandler(std::array<Command *, C> &&commands): commands(commands){};

  CommandStatus handle_input(const nonstd::string_view &input) override {
    auto args = parse_arguments(input);
    if(args.empty()) {
      return CommandStatus::NoCommand;
    }

    const auto command = find_command(args[0]);
    if(command == commands.end()) {
      return CommandStatus::CommandNotFound;
    }
    return (*(*command))(args.size() == 1 ? ArgSpan() : ArgSpan(&args[1], args.size() - 1));
  }

  typename decltype(commands)::const_iterator find_command(const nonstd::string_view &name) const {
    return std::find_if(commands.begin(), commands.end(), [&](const Command *c) {
      return c != nullptr && c->name == name;
    });
  };
};
template<typename... T>
CommandHandler<sizeof...(T)> make_commandhandler(T... commandos) {
  return CommandHandler<sizeof...(T)>({commandos...});
}
}    // namespace Commando
#endif
