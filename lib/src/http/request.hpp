#pragma once

#include <utility>

#include "body.hpp"
#include "headers.hpp"
#include "method.hpp"
#include "uri.hpp"
#include "utility.hpp"
#include "version.hpp"

namespace http
{
    using method_type = http_method::method_type;

    template <typename HttpMethod  = http_method,
              typename HttpUri     = http_uri,
              typename HttpHeaders = http_headers,
              typename HttpBody    = http_body,
              typename HttpVersion = http_version>
    struct http_request
    {

        // member variables
        HttpMethod  _method;
        HttpUri     _uri;
        HttpHeaders _headers;
        HttpBody    _body;
        HttpVersion _version;

        // constructor that takes all of the parameters
        explicit http_request(HttpMethod  && method,
                              HttpUri     && uri,
                              HttpHeaders && headers,
                              HttpBody    && body,
                              HttpVersion && version)
            : _method{std::forward<HttpMethod>(method)}
            , _uri{std::forward<HttpUri>(uri)}
            , _headers{std::forward<HttpHeaders>(headers)}
            , _body{std::forward<HttpBody>(body)}
            , _version{std::forward<HttpVersion>(version)}
        {}

        // constructor that only takes method and uri
        template<typename Method,
                 typename Uri>
        explicit http_request(Method && method, Uri && uri)
            : _method{std::forward<Method>(method)}
            , _uri{std::forward<Uri>(uri)}
        {}

        // rule of five and default constructor
        explicit http_request() = default;
        explicit http_request(http_request const &) = default;
        explicit http_request(http_request &&) = default;

        http_request& operator=(http_request const &) = default;
        http_request& operator=(http_request &&) = default;

        // defaulted virtual destructor in case of inheritance
        virtual ~http_request() = default;

        // method that constructs a new header from the passed arguments
        template<typename ... HttpHeaderField>
        http_request& add_header(HttpHeaderField && ... header_field)
        {
            _headers.add_header(std::forward<HttpHeaderField>(header_field)...);
            return *this;
        }

        // check if all of the fields are set
        bool is_set() const
        {
            return detail::is_set(_method)
                && detail::is_set(_uri)
                && detail::is_set(_version)
                && detail::is_set(_headers)
                && detail::is_set(_body);
        }

    };


    // operator<< for printing http_request to ostream
    // since all fields are public there is no need for this function to be friend
    template<typename OStream, typename HttpMethod,
                               typename HttpUri,
                               typename HttpHeaders,
                               typename HttpBody,
                               typename HttpVersion>
    OStream & operator<<(OStream & out, http_request<HttpMethod,
                                                     HttpUri,
                                                     HttpHeaders,
                                                     HttpBody,
                                                     HttpVersion> const & req)
    {
        if (!req.is_set())
            return out;

        // Status Line
        out << req._method << ' ' << req._uri
                           << ' ' << req._version << "\r\n";

        // Headers
        out << req._headers << "\r\n";

        //Body
        out << req._body;
        
        return out;
    }
} // namespace http
