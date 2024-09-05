#pragma once
#include <concepts>
namespace math::concepts
{
  template <typename T>
  concept IntegeralOrFloat =
      std::is_floating_point<T>::value || std::is_integral<T>::value;

  template <typename T, typename N>
  concept Point = requires(T t, N n) {
    { IntegeralOrFloat<N> };
    { t.x } ;
    { t.y } ;
    { t / n } -> std::same_as<T>;
  };

}