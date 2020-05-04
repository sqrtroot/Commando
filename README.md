<h1><img src="./Commando.svg" width=10% /> Commando </h1>
With commando it is super simple to create commands (small programs) and a shell for all devices.

# How it works
With commando you create commands by making them a subclass of `Commando::Command` and implementing the `operator()` then passing these classes to the CommandHandler which handles calling the correct command and parsing arguments.

# Example
## Command
### Simple command
Let's start by creating a simple hello world command for the arduino
```c++
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
```c++
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
 ```c++
 for(int i = 0; i < args.size(); i++){
     arg[i]; // Do something with the argument
 }
 ```
 ### Why use string views.
 As we could see in the above example there is one downside to using string views. If you use functions that can't accept these but uses `const char *`'s you have to convert them. So why not use `const char *`'s. A long explenation can be found on my blog [#add link](). But a long story short, this is the fastest, non intrusive option. The downside is that when we have to use a null terminated function we have to copy and allocate some memory (by making it a string with `to_str()`), after this the underlying null terminated string can be accessed by using `.c_str()`.

<small>Logo by <a href=https://www.lotts-studio.nl/>lotts studio</a>