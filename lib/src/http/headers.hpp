#pragma once

#include <map>

namespace http
{
    class http_headers
    {
    public:
        http_headers() = default;

        http_headers & add_header(std::string && name, std::string && field)
        {
            headers[name] = field;
            return *this;
        }

    private:
        std::map<std::string, std::string> headers;
    };
} // namespace http
