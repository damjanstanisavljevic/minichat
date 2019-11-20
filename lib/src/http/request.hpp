#pragma once

#include <utility>

namespace http
{   
    template <typename HttpMethod,
              typename HttpUri,
              typename HttpHeaders,
              typename HttpBody,
              typename HttpVersion>
    struct http_request
    {
        http_request() = default;

        http_request(HttpMethod && method, HttpUri && uri, HttpHeaders && headers,
                     HttpBody && body, HttpVersion && version)
            : method(std::forward<HttpMethod>(method))
            , uri(std::forward<HttpUri>(uri))
            , headers(std::forward<HttpHeaders>(headers))
            , version(std::forward<HttpVersion>(version))
            , body(std::forward<HttpBody>(body))
        {}

        template<typename ... HttpHeaderField>
        http_request & add_header(HttpHeaderField && ... header_field)
        {
            headers.add_header(std::forward<HttpHeaderField>(header_field)...);
            return *this;
        }

        HttpMethod  method;
        HttpUri     uri;
        HttpHeaders headers;
        HttpBody    body;
        HttpVersion version;
    };
} // namespace http
