#pragma once

#include <functional>

template<typename T, typename E>
class expected
{
protected:
    union {
        T _value;
        E _error;
    };

    bool _is_valid;

    expected()
    {}

public:
    ~expected()
    {
        if (_is_valid)
            _value.~T();
        else
            _error.~E();
    }

    expected(expected const & other)
        : _is_valid{other._is_valid}
    {
        if (_is_valid)
            new (&_value) T(other._value);
        else
            new (&_error) E(other._error);
    }

    expected(expected && other)
        : _is_valid{other._is_valid}
    {
        if (_is_valid)
            new (&_value) T(std::move(other._value));
        else
            new (&_error) E(std::move(other._error));
    }

    expected &operator= (expected other)
    {
        swap(other);
        return *this;
    }

    void swap(expected & other)
    {
        using std::swap;
        if (_is_valid)
        {
            if (other._is_valid)
            {
                swap(_value, other._value);
            }
            else
            {
                auto temp = std::move(other._error);
                other._error.~E();
                new (&other._value) T(std::move(_value));
                _value.~T();
                new (&_error) E(std::move(temp));
                std::swap(_is_valid, other._is_valid);
            }
        }
        else
        {
            if (other._is_valid)
            {
                other.swap(*this);
            }
            else
            {
                swap(_error, other._error);
                std::swap(_is_valid, other._is_valid);
            }
        }
    }

    template <typename ... ConsParams>
    static expected success(ConsParams && ... params)
    {
        expected result;
        result._is_valid = true;
        new(&result._value) T(std::forward<ConsParams>(params)...);
        return result;
    }

    template <typename ... ConsParams>
    static expected error(ConsParams && ... params)
    {
        expected result;
        result._is_valid = false;
        new(&result._error) E(std::forward<ConsParams>(params)...);
        return result;
    }

    operator bool() const
    {
        return _is_valid;
    }

    bool is_valid() const
    {
        return _is_valid;
    };

    T & get()
    {
        if (!_is_valid)
            std::logic_error("expected<T, E> contains no value");
        return _value;
    }

    T const & get() const
    {
        if (!_is_valid)
            std::logic_error("expected<T, E> contains no value");
        return _value;
    }

    T * operator->()
    {
        return &get();
    }

    T const * operator->() const
    {
        return &get();
    }

    E & error()
    {
        if (_is_valid)
            std::logic_error("There is no error in this expected<T, E>");
        return _error;
    }

    E const & error() const
    {
        if (_is_valid)
            std::logic_error("There is no error in this expected<T, E>");
        return _error;
    }
};

template <typename T,
          typename E,
          typename Function,
          typename ResultType = decltype(std::declval<Function>()(std::declval<T>()))>
ResultType mbind(expected<T, E> const & exp, Function f)
{
    if (exp)
        return std::invoke(f, exp.get());
    else
        return ResultType::error(exp.error());
}
