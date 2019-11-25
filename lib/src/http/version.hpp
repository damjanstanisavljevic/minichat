#pragma once

#include <string>
#include <regex>

namespace http
{
    struct http_version
    {
        http_version()
            : version{"HTTP/1.1"}
        {}

        http_version(int major, int minor)
            : version{"HTTP/"}
        {
            version += major;
            version.push_back('.');
            version += minor;
        }

        explicit http_version(std::string const & s)
            : version{s}
        {}

        explicit http_version(char const * s)
            : version{s}
        {}

        bool is_set() const
        {
            return std::regex_match(version,
                                    std::regex("(HTTP/)([0-9])(.)([0-9])"));
        }

        std::string version;
    };

    template<typename OStream>
    OStream & operator<<(OStream & out, const http_version & version)
    {
        return out << version.version;
    }
} // namespace http
