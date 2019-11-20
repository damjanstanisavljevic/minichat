#pragma once

namespace http
{
    struct http_method
    {
        enum class method_type
        {
            DELETE,
            GET,
            OPTIONS,
            PATCH,
            POST,
            PUT
        };

        http_method()
            : method(method_type::GET)
        {}

        http_method(method_type type)
            : method(type)
        {}

        method_type method;
    };

    template <typename OStream>
    OStream & operator<<(OStream & out, const http_method & method)
    {
        switch (method.method)
        {
            case http_method::method_type::DELETE:
                return out << "DELETE";
            case http_method::method_type::GET:
                return out << "GET";
            case http_method::method_type::OPTIONS:
                return out << "OPTIONS";
            case http_method::method_type::PATCH:
                return out << "PATCH";
            case http_method::method_type::POST:
                return out << "POST";
            default: // case http_method::method_type::PUT:
                return out << "PUT";
        }
    }
} // namespace http
