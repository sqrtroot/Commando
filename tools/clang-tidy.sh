#!/usr/bin/env bash
function do_tidy (){
  echo ====================================
  echo "=$i"
  echo ====================================
  clang-tidy -p . --warnings-as-errors=* $@
  return $?
}

ret_val=0
for i in $(find include/Commando -type f | grep -v include/Commando/Standard); do
  if ! do_tidy $i $@; then
    ret_val=1
  fi
done

for i in $(find src -type f); do
  if ! do_tidy $i $@; then
    ret_val=1
  fi
done

for i in $(find tests -type f -name "*.h" -or -name "*.cpp"); do
  if ! do_tidy $i --checks="-readability-function-size,-cppcoreguidelines-macro-usage" $@; then
    # need to a few checks because of catch macro expansions
    ret_val=1
  fi
done
exit $ret_val
