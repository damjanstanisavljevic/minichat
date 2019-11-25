#pragma once

#include <string>

namespace http
{
    struct http_uri
    {
        http_uri() = default;

        explicit http_uri(const char *u)
            : uri{u}
        {}

        explicit http_uri(std::string u)
            : uri{u}
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
