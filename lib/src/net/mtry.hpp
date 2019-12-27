#pragma once

#include "expected.hpp"

template <typename F,
          typename Ret = typename std::result_of<F()>::type,
          typename Exp = expected<Ret, std::exception_ptr>>
Exp mtry(F f)
{
    try {
        return Exp::success(f());
    } catch (...) {
        return Exp::error(std::current_exception());
    }
}

std::string handle_eptr(std::exception_ptr eptr)
{
    try {
        std::rethrow_exception(eptr);
    } catch(std::exception const & e) {
        return e.what();
    }
}
