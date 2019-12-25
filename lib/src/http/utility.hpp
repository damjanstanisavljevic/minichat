#pragma once

#include <type_traits>
#include <experimental/type_traits>

namespace http
{
    namespace detail
    {
        // helper alias for is_set method
        template<typename T>
        using is_set_method = decltype(&T::is_set);

        // if the type T has method is_set, returns the result of the call of that method
        // if T does not have a method is_set, than it's assumed that there is no need for any
        // special checks so true is returned by default
        template<typename T>
        auto is_set(T const & o)
        {
            if constexpr (std::experimental::is_detected_v<is_set_method,T>)
            {
                return o.is_set();
            }

            return true;
        }
    } // namespace detail
} // namespace http