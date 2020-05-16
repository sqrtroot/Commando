#ifndef COMMANDO_UTILS_TONUMBER_HPP
#define COMMANDO_UTILS_TONUMBER_HPP

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
#include <cstring>
#include <nonstd/optional.hpp>
#include <nonstd/string_view.hpp>

namespace Commando {
namespace Util {
  namespace Impl {

    template<typename T, bool FLOAT_TYPE, bool UNSIGNED_TYPE>
    struct ToNumber {
      static T convert(nonstd::string_view input) { return nonstd::nullopt; };
    };

    template<typename T, bool UNSIGNED_TYPE>
    struct ToNumber<T, true, UNSIGNED_TYPE> {
      static nonstd::optional<T> convert(nonstd::string_view input) {
        const auto str  = input.to_string();
        char *     outp = nullptr;
        const T    out  = std::strtold(str.c_str(), &outp);
        if(outp == str.c_str()) {
          return nonstd::nullopt;
        }
        return out;
      };
    };
    template<typename T>
    struct ToNumber<T, false, true> {
      static nonstd::optional<T> convert(nonstd::string_view input) {
        const auto str  = input.to_string();
        char *     outp = nullptr;
        const T    out  = std::strtoull(str.c_str(), &outp, /*__base=*/10);
        if(outp == str.c_str()) {
          return nonstd::nullopt;
        }
        return out;
      };
    };
    template<typename T>
    struct ToNumber<T, false, false> {
      static nonstd::optional<T> convert(nonstd::string_view input) {
        const auto str  = input.to_string();
        char *     outp = nullptr;
        T          out  = std::strtoll(str.c_str(), &outp, /*__base=*/10);
        if(outp == str.c_str()) {
          return nonstd::nullopt;
        }
        return out;
      };
    };
  }    // namespace Impl
  template<typename T>
  nonstd::optional<T> to_number(nonstd::string_view input) {
    return Impl::ToNumber<T, std::is_floating_point<T>::value, std::is_unsigned<T>::value>::convert(input);
  }
}    // namespace Util
}    // namespace Commando

#endif
