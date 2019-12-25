#pragma once

#include <string>
#include <regex>

namespace http
{
    struct http_version
    {
        // member variables
        std::string _version;
        
        // default constructor initializes the version to "HTTP/1.1"
        explicit http_version()
            : _version{"HTTP/1.1"}
        {}

        // consturcotr that takes two integers major and minor version of the HTTP protocol
        explicit http_version(int major, int minor)
            : _version{"HTTP/"}
        {
            _version += major;
            _version.push_back('.');
            _version += minor;
        }

        // constructor that takes a string
        explicit http_version(std::string const & s)
            : _version{s}
        {}

        // construcotr that takes a const char pointer
        explicit http_version(char const * s)
            : _version{s}
        {}

        //rule of five
        explicit http_version(http_version const &) = default;
        explicit http_version(http_version &&) = default;

        http_version& operator=(http_version const &) = default;
        http_version& operator=(http_version &&) = default;

        // defaulted virtual destructor in case of inheritance
        virtual ~http_version() = default;

        // helper method to check if the version is set
        // version is set if it's in format HTTP/X.Y where X and Y are integers 
        // and X represents major and Y minor version
        bool is_set() const
        {
            return std::regex_match(_version,
                                    std::regex("(HTTP/)([0-9])(.)([0-9])"));
        }

    };

    // operator<< for printing http_version to ostream
    // since all fields are public there is no need for this function to be friend
    template<typename OStream>
    OStream & operator<<(OStream & out, http_version const & version)
    {
        out << version._version;
        return out;
    }
} // namespace http
