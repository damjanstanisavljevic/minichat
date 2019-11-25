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

        bool is_set() const
        {
            return uri != "";
        }

        std::string uri;
    };

    template<typename OStream>
    OStream & operator<<(OStream & out, const http_uri & uri)
    {
        return out << uri.uri;
    }
} // namespace http
