<h1><img src="./Commando.svg" width=10% /> Commando </h1>
With commando it is super simple to create commands (small programs) and a shell for all devices.

![C/C++ CI](https://github.com/sqrtroot/Commando/workflows/C/C++%20CI/badge.svg)
[![codecov](https://codecov.io/gh/sqrtroot/Commando/branch/master/graph/badge.svg)](https://codecov.io/gh/sqrtroot/Commando)

# How it works
With commando you create commands by making them a subclass of `Commando::Command` and implementing the `operator()` then passing these classes to the CommandHandler which handles calling the correct command and parsing arguments.

# Example
## Command
### Simple command
Let's start by creating a simple hello world command for the arduino
```cpp
#include <Commando/Command.hpp>

struct HelloWorld : public Commando::Command {
  HelloWorld(): Command("hello", "hello", "print hello world"){};
  Commando::CommandStatus operator()(const Commando::ArgSpan &args) {
    Serial.println("Hello world");
    return Commando::CommandStatus::Success;
  }
};
```

Calling the constructor of the superclass Command tells the program how the command should be named (and thus how it should be used when called from the commandline), a simple usage for the command which comes in handy if you use some arguments, and a help text.
### Command with arguments
Now let's look at using some arguments. Arguments are passed as `ArgSpan`. This is an alias for a `span<string_view>`. A `span` is like a refference for arrays and can be used as such. A `string_view` is like better implementation of `const char *`'s (they don't need a null terminator). So let's look at how we can implement an echo commando.
```cpp
#include <Commando/Command.hpp>
struct Echo : public Command {
  Echo(): Command("echo", "echo [arguments]", "print input values to serial output"){};
  CommandStatus operator()(const ArgSpan &args) {
    for(const auto &arg: args) {
      Serial.println(arg.to_string().c_str());
    }
    return CommandStatus::Ok;
  }
};
```
 We can see that working with the `ArgSpan` is super simple, we can use it in auto for loops just like arrays/vectors or we could use it with an index like this:
 ```cpp
 for(int i = 0; i < args.size(); i++){
     arg[i]; // Do something with the argument
 }
 ```
 ### Why use string views.
 As we could see in the above example there is one downside to using string views. If you use functions that can't accept these but uses `const char *`'s you have to convert them. So why not use `const char *`'s to begin with? To use those, we have to add null pointers to all the arguments. This would mean either editing the original buffer, or copying the whole buffer. String views bypass this. But the downside is that when we need to use a null terminated strings for a function we have to copy and allocate some memory (by making it a string with `to_str()`), after this the underlying null terminated string can be accessed by using `.c_str()`.

## Command Handler
The command handler is the part that receives the input, splits it into arguments and calls the correct command. Creating a handler is really easy in this example we'll use the previously created `HelloWorld` and `Echo` commands.
```cpp
#include <Commando/CommandHandler.hpp>

EchoCommand       ECHO;
HelloWorldCommand HELLOWORLD;
auto              HANDLER = Commando::make_commandhandler(&ECHO, &HELLOWORLD);
```
 now if we want to call a command we simply call the handler's `handle_input` function like this:
 ```cpp
 HANDLER.handle_input("echo hello world");
 ```
## Shell
The shell class is the easiest way to use commando's.
You give it three function's: a way to check if there's a character to read, a way to read characters and a way to write string_views.
In this example I'll show how to write this yourself.
But for Arduino there is already a standard shell defined that you can use as such (with the handler from the previous example):
```cpp
#include <Commando/StandardShell/Arduino.hpp>

ArduinoShell SHELL(HANDLER);

void loop(){
  SHELL.step();
}
```
### How to write it yourself
So as said before you need to provide three functions. These functions can be written as lambdas.
Let's look at a shell that uses ```std::cin``` to read characters
```cpp
#include <Commando/Commando.h>
#include <iostream>

int main() {
  auto handler = Commando::make_commandhandler();
  std::cin.sync_with_stdio(false);//make sure rdbuf in avail works
  Commando::Shell shell(
    handler,
    []() { return std::cin.rdbuf()->in_avail() > 0; },
    []() { return std::cin.get(); },
    [](nonstd::string_view sv) {std::cout << sv;},
    Commando::Shell::NoEcho);
}
```
Our first lambda checks if the cin buffer holds at leas one character. There is line before creating the shell `std::cin.sync_with_stdio(false);`. This makes sure we can check if there's a character available with standard c++ functions.
The second lambda just returns a character from the cin string.
The third lambda prints the string view we recieve to the standard output.
And there we have it. The 3 functions to write to make a complete shell.

As a small aside. There is a fourth optional parameter, which indicates if you need to write the incomming character to the stream. This is mostly usefull for streams that don't have local echo enabled, like some serial monitors. For our iostream based shell this is not needed so we pass an extra argument, indicating this.

# Todo
- [x] Add a shell for arduino
- [ ] Test default arduino commando's
- [ ] Make library build for esp
- [ ] Add wifi commando's


<small>Logo by <a href=https://www.lotts-studio.nl/>lotts studio</a>
