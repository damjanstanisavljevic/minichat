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

        template<typename OStream>
        friend OStream & operator<<(OStream & out, const http_headers & headers);

    private:
        std::map<std::string, std::string> headers;
    };

    template<typename OStream>
    OStream & operator<<(OStream & out, const http_headers & headers)
    {
        for (auto const & [name, field] : headers.headers)
            out << name << ": " << field << "\r\n";
        return out;
    }
} // namespace http
