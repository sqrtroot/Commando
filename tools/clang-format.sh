#!/bin/bash

function do_format (){
  echo ====================================
  echo "=$i"
  echo ====================================
  clang-format -n -Werror $@
  return $?
}

ret_val=0
for i in $(find include/Commando -type f | grep -v include/Commando/Standard); do
  if ! do_format $i $@; then
    ret_val=1
  fi
done

for i in $(find src -type f); do
  if ! do_format $i $@; then
    ret_val=1
  fi
done

for i in $(find tests -type f -name "*.h" -or -name "*.cpp"); do
  if ! do_format $i $@; then
    ret_val=1
  fi
done
exit $ret_val
