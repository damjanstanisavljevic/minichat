#pragma once

namespace http
{
    struct http_method
    {
        enum method_type
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
} // namespace http
