#pragma once

namespace http
{
    struct http_status_code
    {
        enum class status_code_type
        {
            OK          = 200,
            Bad_Request = 400
        };

        // member varialbes
        status_code_type _status_code;
        

        // default constructor default initalizes to method_type::GET
        explicit http_status_code()
            : _status_code{status_code_type::OK}
        {}

        // constructor that takes http_status_code::status_code_type as argument
        http_status_code(status_code_type type)
            : _status_code{type}
        {}

        // constructor that takes an int as argument
        http_status_code(int code)
            : _status_code{code}
        {}

        // assigment operator that assignes from the http_method::method_type
        http_status_code & operator=(status_code_type type)
        {
            _status_code = type;
            return *this;
        }

        // assigment operator that assignes from the std::string
        void operator=(std::string const & type)
        {
            if (type == "OK")
                _status_code = status_code_type::OK;
            else if (type == "Bad Request")
                _status_code = status_code_type::Bad_Request;
        }

        void operator=(int code)
        {
            _status_code = static_cast<status_code_type>(code);
        }

        // rule of five
        explicit http_status_code(http_status_code const &) = default;
        explicit http_status_code(http_status_code &&) = default;

        http_status_code& operator=(http_status_code const &) = default;
        http_status_code& operator=(http_status_code &&) = default;

        // default virtual destructor in case of inheritance
        virtual ~http_status_code() = default;

    };


    // operator<< for printing http_method
    template <typename OStream>
    OStream & operator<<(OStream & out, http_status_code const & status_code)
    {
        switch (status_code._status_code)
        {
            case http_status_code::status_code_type::OK:
                out << "200 OK";
                break;
            default: // case http_status_code::status_code_type::Bad_Request:
                out << "400 Bad Request";
                break;
        }
        return out;
    }
} // namespace http