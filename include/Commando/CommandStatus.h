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
#ifndef COMMANDO_COMMANDSTATUS_H
#define COMMANDO_COMMANDSTATUS_H

namespace Commando {
enum CommandStatus {
  // Parse Errors
  CommandNotFound = -99,
  NoCommand       = -98,

  // Success
  Success = 0,
  Ok      = 0,

  // Execution Errors
  NotEnoughArguments      = 1,
  InvalidArgument         = 2,
  InvalidNumberConversion = 3,
};
}    // namespace Commando

#endif    // COMMANDO_COMMANDSTATUS_H
