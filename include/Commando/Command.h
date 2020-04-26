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
#ifndef COMMANDO_COMMAND_H
#define COMMANDO_COMMAND_H
#include "ArgSpan.h"
#include "CommandStatus.h"

#include <cstdint>
#include <cstring>
#include <iterator>

namespace Commando {
struct Command {
  /** Name of the command, this is also how you call the command */
  const char *name;
  /**
   * Usage of the command.
   * @example digitalWrite [pin] [value]
   */
  const char *usage;
  /** Help text of the command */
  const char *help = nullptr;

  constexpr Command(const char *name, const char *usage): name(name), usage(usage) {}

  constexpr Command(const char *name, const char *usage, const char *help):
      name(name), usage(usage), help(help) {}

  /**
   * This function is executed when the command is called
   * @param args Arguments passed from the command line
   * @return exit status of the command
   */
  virtual CommandStatus operator()(const ArgSpan &args) = 0;

  bool operator==(const Command &rhs) const { return strcmp(name, rhs.name) == 0; }
  bool operator!=(const Command &rhs) const { return !(rhs == *this); }
};
}    // namespace Commando
#endif
