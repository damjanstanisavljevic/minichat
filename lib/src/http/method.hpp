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

        void operator=(method_type type)
        {
            method = type;
        }

        void operator=(std::string const & type)
        {
            if (type == "DELETE")
                method = method_type::DELETE;
            else if (type == "GET")
                method = method_type::GET;
            else if (type == "OPTIONS")
                method = method_type::OPTIONS;
            else if (type == "PATCH")
                method = method_type::PATCH;
            else if (type == "POST")
                method = method_type::POST;
            else if (type == "PUT")
                method = method_type::PUT;
        }

        method_type method;
    };

    template <typename OStream>
    OStream & operator<<(OStream & out, const http_method & method)
    {
        switch (method.method)
        {
            case http_method::method_type::DELETE:
                out << "DELETE";
                break;
            case http_method::method_type::GET:
                out << "GET";
                break;
            case http_method::method_type::OPTIONS:
                out << "OPTIONS";
                break;
            case http_method::method_type::PATCH:
                out << "PATCH";
                break;
            case http_method::method_type::POST:
                out << "POST";
                break;
            default: // case http_method::method_type::PUT:
                out << "PUT";
                break;
        }
        return out;
    }
} // namespace http
