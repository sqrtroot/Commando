#include <Commando/Commando.h>
#include <catch2/catch.hpp>

SCENARIO("Argument string should be parsed into seperate arguments") {
  GIVEN("A simple argument") {
    auto input = "Hello world";
    WHEN("It's parsed") {
      auto parsed = Commando::CommandHandlerBase::parse_arguments(input);
      THEN("It should have the correct number of arguments") {
        REQUIRE(parsed.size() == 2);
      }
      THEN("It should have the correct arguments") {
        REQUIRE(parsed.at(0) == "Hello");
        REQUIRE(parsed.at(1) == "world");
      }
    }
  }
  GIVEN("A double quoted argument") {
    auto input = "\"Hello world\"";
    WHEN("It's parsed") {
      auto parsed = Commando::CommandHandlerBase::parse_arguments(input);
      THEN("It should have the correct number of arguments") {
        REQUIRE(parsed.size() == 1);
      }
      THEN("It should have the correct arguments") {
        REQUIRE(parsed.at(0) == "Hello world");
      }
    }
  }
  GIVEN("A quoted argument") {
    auto input = "'Hello world'";
    WHEN("It's parsed") {
      auto parsed = Commando::CommandHandlerBase::parse_arguments(input);
      THEN("It should have the correct number of arguments") {
        REQUIRE(parsed.size() == 1);
      }
      THEN("It should have the correct arguments") {
        REQUIRE(parsed.at(0) == "Hello world");
      }
    }
  }
}

SCENARIO("Commands should be executed") {
  GIVEN("A commando and a command handler") {
    struct TestCommand : public Commando::Command {
      bool        executed = false;
      std::string arg1;
      std::string arg2;
      TestCommand(): Commando::Command("test", ""){};
      Commando::CommandStatus operator()(const Commando::ArgSpan &args) {
        executed = true;
        if(args.empty()) {
          return Commando::CommandStatus::NotEnoughArguments;
        }
        arg1 = args[0].to_string();
        arg2 = args[1].to_string();
        return Commando::CommandStatus::Ok;
      }
    };
    TestCommand                 t1;
    Commando::CommandHandler<1> ch({&t1});
    WHEN("The handle input function is called with the name of the command") {
      ch.handle_input("test");
      THEN("The command should be executed") { REQUIRE(t1.executed); }
    }
    WHEN(
      "The handle input function is called with the name of the command and "
      "arguments") {
      ch.handle_input("test hello world");
      THEN("The arguments should have been passed correctly") {
        REQUIRE(t1.arg1 == "hello");
        REQUIRE(t1.arg2 == "world");
      }
    }
    WHEN("Not enough arguments are given ") {
      auto res = ch.handle_input("test");
      THEN("The error created by the command should be passed through") {
        REQUIRE(res == Commando::CommandStatus::NotEnoughArguments);
      }
    }
    WHEN("No input is given") {
      auto res = ch.handle_input("");
      THEN("The result should be the nocommand error") {
        REQUIRE(res == Commando::CommandStatus::NoCommand);
      }
    }
    WHEN("An nonexisting command is given") {
      auto res = ch.handle_input("asdf");
      THEN("The result should be the CommandNotFound error") {
        REQUIRE(res == Commando::CommandStatus::CommandNotFound);
      }
    }
  }
}
