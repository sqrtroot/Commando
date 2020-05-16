#include "Commando/CommandHandler.h"
#include <algorithm>
#include <cctype>
#include <iterator>
#include <nonstd/string_view.hpp>
#include <vector>

constexpr auto QUOTED_NEXT_OFFSET = 2;

bool is_argpart(const char c) {
  return (std::isprint(c) != 0) && c != ' ';
}

nonstd::string_view::const_iterator find_start(const nonstd::string_view input) {
  auto current = input.begin();
  while(current != input.end() && !is_argpart(*current)) {
    current++;
  }
  return current;
}

nonstd::string_view parse_quoted(const nonstd::string_view input, char quote) {
  auto current = input.begin();
  while(*current != quote && current != input.end()) {
    current++;
  }
  return nonstd::string_view(input.begin(), std::distance(input.begin(), current));
}
nonstd::string_view parse_unquoted(const nonstd::string_view input) {
  auto current = input.begin();
  while(is_argpart(*current) && current != input.end()) {
    current++;
  }
  return nonstd::string_view(input.begin(), std::distance(input.begin(), current));
}

namespace Commando {
std::vector<nonstd::string_view>
  CommandHandlerBase::parse_arguments(const nonstd::string_view input) {
  auto                             current = input.begin();
  std::vector<nonstd::string_view> arguments;
  while(current < input.end()) {
    if(*current == '"' || *current == '\'') {
      current++;
      auto result = parse_quoted(
        nonstd::string_view(current, std::distance(current, input.end())), *(current - 1));
      arguments.push_back(result);
      current = result.end() + QUOTED_NEXT_OFFSET;
    } else {
      auto result =
        parse_unquoted(nonstd::string_view(current, std::distance(current, input.end())));
      arguments.push_back(result);
      current = result.end() + 1;
    }
  }
  return arguments;
}
}    // namespace Commando
