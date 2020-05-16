#include <Commando/Commando.h>
#include <TestCommand.h>
#include <catch2/catch.hpp>

SCENARIO("A shell should read input") {
  auto        char_ready       = false;
  auto        char_read_called = false;
  auto        return_char      = '-';
  std::string last_output;
  auto        t1    = TestCommand();
  auto        ch    = Commando::make_commandhandler(&t1);
  auto        shell = Commando::Shell(
    &ch,
    [&]() { return char_ready; },
    [&]() {
      char_read_called = true;
      return return_char;
    },
    [&](nonstd::string_view out) { last_output = out.to_string(); });

  GIVEN("A shell with functions to read characters") {
    WHEN("Step is called") {
      AND_WHEN("No character is ready") {
        THEN("Char read should not be called") {
          shell.step();
          REQUIRE(!char_read_called);
        }
      }
      AND_WHEN("A character is ready") {
        char_ready = true;
        shell.step();
        THEN("Char read should be called") { REQUIRE(char_read_called); }
        AND_THEN("The output should be printed") {
          REQUIRE(last_output == std::string(1, return_char));
        }
      }
      AND_WHEN("Echo character is disabled no character should be echoed") {
        auto shell2 = Commando::Shell(
          &ch,
          [&]() { return char_ready; },
          [&]() {
            char_read_called = true;
            return return_char;
          },
          [&](nonstd::string_view out) { last_output = out.to_string(); },
          Commando::Shell::NO_ECHO);
        char_ready = true;
        shell2.step();
        THEN("The output should NOT be printed") { REQUIRE(last_output.empty()); }
      }
    }
  }
  WHEN("A character is added to the buffer") {
    return_char = 'a';
    char_ready  = true;
    shell.step();
    THEN("The buffer should contain this character") {
      REQUIRE(shell.buffer == std::string(1, return_char));
    }
    AND_WHEN("A backspace is called") {
      return_char = '\b';
      char_ready  = true;
      shell.step();
      THEN("The character should be removed") { REQUIRE(shell.buffer.empty()); }
      AND_THEN("The string to remove a letter should be printed") {
        REQUIRE(last_output == shell.remove_char);
      }
    }
  }
}

SCENARIO("A shell should execute commands") {
  std::string input_string = "test\n";
  std::string last_output;
  auto        t1    = TestCommand();
  auto        ch    = Commando::make_commandhandler(&t1);
  auto        shell = Commando::Shell(
    &ch,
    [&]() { return !input_string.empty(); },
    [&]() {
      auto letter = input_string.front();
      input_string.erase(/*__pos=*/0, /*__n=*/1);
      return letter;
    },
    [&](nonstd::string_view out) { last_output = out.to_string(); });

  GIVEN("A shell with an complete input") {
    WHEN("All characters are read") {
      while(!input_string.empty()) {
        shell.step();
      }
      THEN("The command should be executed") { REQUIRE(t1.executed); }
      AND_THEN("The command leader should be printed") {
        REQUIRE(last_output == shell.leader);
      }
    }
  }
}