#ifndef COMMANDO_TOBASECLASS_H
#define COMMANDO_TOBASECLASS_H
#include <array>
#include <cstddef>
namespace Impl {

template<typename R, size_t S, size_t I>
struct ToBaseclass {
  template<typename... T>
  constexpr std::array<R, S> operator()(const std::tuple<T...> &r, std::array<R, S> &&arr) const {
    return arr[S - I] = static_cast<R>(&std::get<S - I>(r)),
                   ToBaseclass<R, S, I - 1>()(r, std::move(arr));
  }
};

template<typename R, size_t S>
struct ToBaseclass<R, S, 0> {
  template<typename... T>
  constexpr std::array<R, S>
    operator()(const std::tuple<T...> & /*unused*/, std::array<R, S> &&arr) const {
    return arr;
  }
};
}    // namespace Impl

template<typename Base, typename... T>
constexpr std::array<const Base *, sizeof...(T)> to_baseclass(const std::tuple<T...> &t) {
  return Impl::ToBaseclass<const Base *, sizeof...(T), sizeof...(T)>()(
    t, std::array<const Base *, sizeof...(T)>());
}

template<typename Base, typename... T>
constexpr std::array<Base *, sizeof...(T)> to_baseclass(std::tuple<T...> &t) {
  return Impl::ToBaseclass<Base *, sizeof...(T), sizeof...(T)>()(
    t, std::array<Base *, sizeof...(T)>());
}

#endif
