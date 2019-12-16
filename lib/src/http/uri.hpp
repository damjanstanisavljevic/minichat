#pragma once

#include <string>

namespace http
{
    struct http_uri
    {

        // member variables
        std::string _uri;

        // constructor that takes a const char pointer
        explicit http_uri(char const * u)
            : _uri{u}
        {}

        // constructor that takes a string_view
        explicit http_uri(std::string_view const & u)
            : _uri{u}
        {}

        // assigment operator that takes a const char pointer
        http_uri& operator=(const char *u)
        {
            _uri = u;
            return *this;
        }
        
        // assigment operator that thakes a string
        http_uri& operator=(std::string const & u)
        {
            _uri = u;
            return *this;
        }

        // rule of five and default constructor
        explicit http_uri() = default;
        explicit http_uri(http_uri const &) = default;
        explicit http_uri(http_uri && ) = default;

        http_uri& operator=(http_uri const &) = default;
        http_uri& operator=(http_uri &&) = default;

        // defaulted virtula destructor in case of inheritance
        virtual ~http_uri() = default;

        // is_set helper method to check if the uri is set
        // returns true if uri isn't an empty string
        bool is_set() const
        {
            return _uri != "";
        }

    };

    // operator<< for printing http_uri to ostream
    // since all fields are public there is no need for this function to be friend
    template<typename OStream>
    OStream & operator<<(OStream & out, http_uri const & uri)
    {
        return out << uri._uri;
    }
} // namespace http
