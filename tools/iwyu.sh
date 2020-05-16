#!/bin/bash
iwyu=$(iwyu_tool.py -p . -- -Xiwyu --no_comments -Xiwyu --quoted_includes_first -Xiwyu --check_also="*/include/Commando/*.h")
printf "$iwyu\n"
if ! echo $iwyu | grep -o -q -e "remove" -e "add"; then
  exit 0
else
  exit 1
fi
