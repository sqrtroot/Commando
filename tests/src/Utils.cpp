#include <Commando/Utils/ToNumber.hpp>
#include <catch2/catch.hpp>
#include <limits>
#include <nonstd/optional.hpp>
#include <sstream>

// MSVC Doesn't accept chars/unsigned chars as random type,
// so we need to check for msvc and generate larger values
// that we then truncate to the smaller type
template<typename T>
struct MinRandomType {
  using type = T;
};
#ifdef _WIN32
template<>
struct MinRandomType<char> {
  using type = short;
};
template<>
struct MinRandomType<unsigned char> {
  using type = unsigned short;
};
#endif

#define GIVEN_NUM_TYPE(T, value_comparison)                                             \
  GIVEN("type " #T) {                                                                   \
    using randomT = MinRandomType<T>::type;                                             \
    using limits  = std::numeric_limits<randomT>;                                       \
    WHEN("A correct number is decoded") {                                               \
      auto value = (T) GENERATE(                                                        \
        limits::min(), limits::max(), take(100, random(limits::min(), limits::max()))); \
      std::stringstream ss;                                                             \
      ss << +value;                                                                     \
      auto decoded = Commando::Util::to_number<T>(ss.str());                            \
      THEN("it should hold a value") { REQUIRE(decoded.has_value()); }                  \
      AND_THEN("The value should be correct: " << value) { value_comparison }           \
    }                                                                                   \
    WHEN("A incorrect number is decoded") {                                             \
      auto decoded = Commando::Util::to_number<T>("oidsjfoaisjdfoiajsdf");              \
      THEN("It should not hold a value") { REQUIRE(!decoded.has_value()); }             \
    }                                                                                   \
  }

#define GIVEN_INT_TYPE(T) GIVEN_NUM_TYPE(T, REQUIRE(decoded.value() == value);)
#define GIVEN_FLOAT_TYPE(T) \
  GIVEN_NUM_TYPE(T, REQUIRE(decoded.value() == Approx(value));)

SCENARIO("String views should be convertible to numbers") {
  GIVEN_INT_TYPE(char);
  GIVEN_INT_TYPE(short);
  GIVEN_INT_TYPE(int);
  GIVEN_INT_TYPE(long);
  GIVEN_INT_TYPE(long long);

  GIVEN_INT_TYPE(unsigned char);
  GIVEN_INT_TYPE(unsigned short);
  GIVEN_INT_TYPE(unsigned int);
  GIVEN_INT_TYPE(unsigned long);
  GIVEN_INT_TYPE(unsigned long long);

  GIVEN_FLOAT_TYPE(float);
  GIVEN_FLOAT_TYPE(double);
  GIVEN_FLOAT_TYPE(long double);
}
