#ifndef TESTCOMMAND_H
#define TESTCOMMAND_H

#include <Commando/Commando.h>
struct TestCommand : public Commando::Command {
  bool        executed = false;
  std::string arg1;
  std::string arg2;
  TestCommand(): Commando::Command("test", ""){};
  Commando::CommandStatus operator()(const Commando::ArgSpan &args) override {
    executed = true;
    if(args.empty()) {
      return Commando::CommandStatus::NotEnoughArguments;
    }
    arg1 = args[0].to_string();
    arg2 = args[1].to_string();
    return Commando::CommandStatus::Ok;
  }
};
#endif
