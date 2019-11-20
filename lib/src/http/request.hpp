#pragma once

namespace http
{
    enum class http_method
    {
        GET,
        POST,
        PATCH,
        PUT,
        DELETE,
        OPTIONS
    };
    
    template <typename HttpMethod,
              typename HttpUri,
              typename HttpHeaders,
              typename HttpBody,
              typename HttpVersion>
    struct http_request
    {
        http_request() = default;

        http_request(HttpMethod method, HttpUri uri, HttpHeaders headers,
                     HttpBody body, HttpVersion version = "HTTP/1.1")
            : method(method)
            , uri(uri)
            , headers(headers)
            , version(version)
            , body(body)
        {}

        HttpMethod  method;
        HttpUri     uri;
        HttpHeaders headers;
        HttpBody    body;
        HttpVersion version;
    };
}