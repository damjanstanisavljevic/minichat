#pragma once

namespace http
{
    struct http_method
    {
        // enum of all supported method_types
        enum class method_type
        {
            DELETE,
            GET,
            OPTIONS,
            PATCH,
            POST,
            PUT
        };

        // member varialbes
        method_type _method;

        // deafailt constructor deafault initalizes to method_type::GET
        explicit http_method()
            : _method{method_type::GET}
        {}

        // constructor that takes http_method::method_type as argument
        http_method(method_type type)
            : _method{type}
        {}

        // assigment operator that assignes from the http_method::method_type
        http_method& operator=(method_type type)
        {
            _method = type;
            return *this;
        }

        // assigment operator that assignes from the std::string
        void operator=(std::string const & type)
        {
            if (type == "DELETE")
                _method = method_type::DELETE;
            else if (type == "GET")
                _method = method_type::GET;
            else if (type == "OPTIONS")
                _method = method_type::OPTIONS;
            else if (type == "PATCH")
                _method = method_type::PATCH;
            else if (type == "POST")
                _method = method_type::POST;
            else if (type == "PUT")
                _method = method_type::PUT;
        }

        // rule of five
        explicit http_method(http_method const &) = default;
        explicit http_method(http_method &&) = default;

        http_method& operator=(http_method const &) = default;
        http_method& operator=(http_method &&) = default;

        // deafult virtual destructor in case of inheritance
        virtual ~http_method() = default;

    };


    // operator<< for printing http_method
    template <typename OStream>
    OStream & operator<<(OStream & out, http_method const & method)
    {
        switch (method._method)
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
