#pragma once

#include "body.hpp"
#include "code.hpp"
#include "headers.hpp"
#include "utility.hpp"
#include "version.hpp"

namespace http
{
    using status_code_type = http_status_code::status_code_type;

    template <typename HttpStatusCode = http_status_code,
              typename HttpVersion    = http_version,
              typename HttpHeaders    = http_headers,
              typename HttpBody       = http_body>
    struct http_response
    {

        // member variables
        HttpStatusCode _status_code;
        HttpVersion    _version;
        HttpHeaders    _headers;
        HttpBody       _body;

        // constructor that takes all of the parameters
        explicit http_response(HttpStatusCode && status_code,
                               HttpVersion    && version,
                               HttpHeaders    && headers,
                               HttpBody       && body)
            : _status_code{std::forward<HttpStatusCode>(status_code)}
            , _version{std::forward<HttpVersion>(version)}
            , _headers{std::forward<HttpHeaders>(headers)}
            , _body{std::forward<HttpBody>(body)}
        {}

        // constructor that only takes the status code
        template <typename StatusCode>
        explicit http_response(StatusCode && status_code)
            : _status_code{std::forward<StatusCode>(status_code)}
        {}

        // rule of five and default constructor
        explicit http_response() = default;
        explicit http_response(http_response const &) = default;
        explicit http_response(http_response &&) = default;

        http_response& operator=(http_response const &) = default;
        http_response& operator=(http_response &&) = default;

        // defaulted virtual destructor in case of inheritance
        virtual ~http_response() = default;

        // method that constructs a new header from the passed arguments
        template<typename ... HttpHeaderField>
        http_response& add_header(HttpHeaderField && ... header_field)
        {
            _headers.add_header(std::forward<HttpHeaderField>(header_field)...);
            return *this;
        }

        // check if all of the fields are set
        bool is_set() const
        {
            return detail::is_set(_status_code)
                && detail::is_set(_version)
                && detail::is_set(_headers)
                && detail::is_set(_body);
        }

        
    };

    // operator<< for printing http_response to ostream
    // since all fields are public there is no need for this function to be friend
    template<typename OStream, typename HttpStatusCode,
                               typename HttpVersion,
                               typename HttpHeaders,
                               typename HttpBody>
    OStream & operator<<(OStream & out, http_response<HttpStatusCode,
                                                      HttpVersion,
                                                      HttpHeaders,
                                                      HttpBody> const & res)
    {
        if (!res.is_set())
            return out;

        // Status Line
        out << res._version << " " << res._status_code << "\r\n";

        // Headers
        out << res._headers << "\r\n";

        //Body
        out << res._body;
        
        return out;
    }
} // namespace http