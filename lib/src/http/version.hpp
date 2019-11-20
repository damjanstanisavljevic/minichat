#pragma once

#include <string>

namespace http
{
    struct http_version
    {
        http_version(int major, int minor)
            : version("HTTP/")
        {
            version += major;
            version.push_back('.');
            version += minor;
        }

        http_version(std::string s)
            : version(s)
        {}

        std::string version;
    };
}