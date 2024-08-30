#pragma once

namespace math::concepts
{
    template <typename T>
    concept IntegeralOrFloat =
        std::is_floating_point<T>::value || std::is_integral<T>::value;

    template <typename T>
    concept Point = requires(T t) {
        { t.x }
        { t.y }
        t / IntegeralOrFloat = std::same_as<T>
    }

}