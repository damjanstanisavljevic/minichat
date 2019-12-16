#pragma once

#include "../../include/json.hpp"

namespace http
{
    // Simple struct to represnt the body of the http_packet
    struct http_body
    {
        // member variables
        nlohmann::json _json;

        // rule of five and default constructor
        explicit http_body() = default;
        explicit http_body(http_body const &) = default;
        explicit http_body(http_body && ) = default;

        http_body& operator=(http_body const &) = default;
        http_body& operator=(http_body &&) = default;

        // default virtual destructor in case of inheritance
        virtual ~http_body() = default;

        // constructor that takes a json or anything that is convertible to JSON
        template<typename T,
                 typename = std::enable_if_t<std::is_constructible_v<nlohmann::json, T>> >
        explicit http_body(T && t) : _json{std::forward<T>(t)}
        {}

    };


    // operator<< for printing the http_body
    // since all fields are public there is no need to make it a friend funciton
    template<typename OStream>
    OStream & operator<<(OStream & out, http_body const & body)
    {
        if (!body._json.empty())
            out << body._json;
        return out;
    }
} // namespace http
