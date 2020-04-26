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
#ifndef COMMANDO_ARDUINO_HPP
#define COMMANDO_ARDUINO_HPP
#include <Arduino.h>
#include <Commando/Commando.h>
#include <Commando/Utils/ToNumber.hpp>
#include <nonstd/optional.hpp>
using namespace Commando;

struct PinMode : public Command {
  PinMode():
      Command("pinMode",

              "pinMode [pinNumber] [mode]",

              "Set the mode of a pin"
              "mode can be one of INPUT, INPUT_PULLUP, OUTPUT") {}

  nonstd::optional<uint32_t> to_pinmode(const nonstd::string_view view) {
    if(view == "INPUT") return INPUT;
    if(view == "INPUT_PULLUP") return INPUT_PULLUP;
    if(view == "OUTPUT") return OUTPUT;
    return nonstd::nullopt;
  }

  CommandStatus operator()(const ArgSpan &argSpan) {
    if(argSpan.size() < 2) return CommandStatus::NotEnoughArguments;

    auto pin = Util::to_number<int>(argSpan[0]);
    if(!pin.has_value()) return CommandStatus::InvalidNumberConversion;

    auto desiredPinMode = to_pinmode(argSpan[1]);
    if(!desiredPinMode.has_value()) return CommandStatus::InvalidArgument;

    pinMode(pin.value(), desiredPinMode.value());
    return CommandStatus::Ok;
  }
};

struct AnalogRead : public Command {
  Stream &stream;
  AnalogRead(): AnalogRead(Serial){};
  AnalogRead(Stream &stream):
      Command("analogRead",
              "analogRead [pin]",
              "read an analog pin and write value to stream"),
      stream(stream){};

  CommandStatus operator()(const ArgSpan &argSpan) {
    if(argSpan.size() < 1) return CommandStatus::NotEnoughArguments;

    auto pin = Util::to_number<int>(argSpan[0]);
    if(!pin.has_value()) return CommandStatus::InvalidNumberConversion;

    stream.println(analogRead(pin.value()));
    return CommandStatus::Ok;
  }
};

struct DigitalRead : public Command {
  Stream &stream;
  DigitalRead(): DigitalRead(Serial){};
  DigitalRead(Stream &stream):
      Command("digitalRead",
              "digitalRead [pin]",
              "read the value of a pin and write value to stream"),
      stream(stream){};

  CommandStatus operator()(const ArgSpan &argSpan) {
    if(argSpan.size() < 1) return CommandStatus::NotEnoughArguments;

    auto pin = Util::to_number<int>(argSpan[0]);
    if(!pin.has_value()) return CommandStatus::InvalidNumberConversion;

    stream.println(digitalRead(pin.value()));
    return CommandStatus::Ok;
  }
};

struct DigitalWrite : public Command {
  DigitalWrite():
      Command("digitalWrite",
              "digitalWrite [pin] [value]",
              "write a value to a digital pin, value can be HIGH, LOW, 0 or 1"){};

  nonstd::optional<int> to_value(nonstd::string_view view) {
    if(view == "HIGH") return HIGH;
    if(view == "LOW") return LOW;
    if(view == "1") return HIGH;
    if(view == "0") return LOW;
    return nonstd::nullopt;
  }
  CommandStatus operator()(const ArgSpan &argSpan) {
    if(argSpan.size() < 2) return CommandStatus::NotEnoughArguments;

    auto pin = Util::to_number<int>(argSpan[0]);
    if(!pin.has_value()) return CommandStatus::InvalidNumberConversion;

    auto value = to_value(argSpan[1]);
    if(!value.has_value()) return CommandStatus::InvalidArgument;

    digitalWrite(pin.value(), value.value());
    return CommandStatus::Ok;
  }
};

struct AnalogWrite : public Command {
  AnalogWrite():
      Command("analogWrite",
              "analogWrite [pin] [value]",
              "Write a value to a adc pin, or use pwm.\nValue can be HIGH, LOW, 0 "
              "or 1"){};

  nonstd::optional<int> to_value(nonstd::string_view view) {
    if(view == "HIGH") return HIGH;
    if(view == "LOW") return LOW;
    return Util::to_number<int>(view);
  }

  CommandStatus operator()(const ArgSpan &argSpan) {
    if(argSpan.size() < 2) return CommandStatus::NotEnoughArguments;

    auto pin = Util::to_number<int>(argSpan[0]);
    if(!pin.has_value()) return CommandStatus::InvalidNumberConversion;

    auto value = to_value(argSpan[1]);
    if(!value.has_value()) return CommandStatus::InvalidArgument;
    analogWrite(pin.value(), value.value());
    return CommandStatus::Ok;
  }
};

struct Echo : public Command {
  Echo(): Command("echo", "echo [values]", "echo input values"){};

  CommandStatus operator()(const ArgSpan &argSpan) {
    for(const auto &value: argSpan) {
      Serial.println(value.to_string().c_str());
    }
    return CommandStatus::Ok;
  }
};

#endif    // COMMANDLIB_ARDUINO_HPP
