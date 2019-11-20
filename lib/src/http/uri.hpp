#pragma once

#include <string>

namespace http
{
    struct http_uri
    {
        http_uri() = default;

        http_uri(const char *uri)
            : uri(uri)
        {}

        http_uri(std::string uri)
            : uri(uri)
        {}

        std::string uri;
    };
}