#pragma once

#include <utility>

#include "headers.hpp"
#include "method.hpp"
#include "uri.hpp"
#include "version.hpp"

namespace http
{   
    template <typename HttpMethod  = http_method,
              typename HttpUri     = http_uri,
              typename HttpHeaders = http_headers,
              typename HttpBody    = std::string,
              typename HttpVersion = http_version>
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

    template<typename OStream, typename HttpMethod,
                               typename HttpUri,
                               typename HttpBody,
                               typename HttpHeaders,
                               typename HttpVersion>
    OStream & operator<<(OStream & out, http_request<HttpMethod,
                                                      HttpUri,
                                                      HttpBody,
                                                      HttpHeaders,
                                                      HttpVersion> const & request)
    {
        // Status Line
        out << request.method << ' ' << request.uri
                              << ' ' << request.version << "\r\n";

        // Headers
        out << request.headers << "\r\n";

        //Body
        out << request.body;
    }
} // namespace http
