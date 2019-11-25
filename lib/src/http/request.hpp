#pragma once

#include <type_traits>
#include <utility>

#include "headers.hpp"
#include "method.hpp"
#include "uri.hpp"
#include "version.hpp"
#include "body.hpp"

namespace http
{
    namespace detail
    {
        template<typename T>
        class has_is_set
        {
        private:
            template<typename U, bool (U::*)() const> struct check;
            template<typename U> static char func(check<U, &U::is_set> *);
            template<typename U> static int  func(...);
        public:
            typedef has_is_set type;
            enum { value = sizeof(func<T>(0)) == sizeof(char) };
        };

        template<typename T>
        bool is_set_if_exists(T const & o, std::true_type)
        {
            return o.is_set();
        }

        template<typename T>
        constexpr bool is_set_if_exists(T const &, std::false_type)
        {
            return true;
        }

        template<typename T>
        bool is_set(T const & o)
        {
            return is_set_if_exists(o,
                    std::integral_constant<bool,
                        has_is_set<typename std::decay_t<T>>::value>());
        }
    } // namespace detail

    template <typename HttpMethod  = http_method,
              typename HttpUri     = http_uri,
              typename HttpHeaders = http_headers,
              typename HttpBody    = http_body,
              typename HttpVersion = http_version>
    struct http_request
    {
        http_request() = default;

        http_request(HttpMethod  && method,
                     HttpUri     && uri,
                     HttpHeaders && headers,
                     HttpBody    && body,
                     HttpVersion && version)
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

        bool is_set() const
        {
            return detail::is_set(method)
                && detail::is_set(uri)
                && detail::is_set(version)
                && detail::is_set(headers)
                && detail::is_set(body);
        }

        HttpMethod  method;
        HttpUri     uri;
        HttpHeaders headers;
        HttpBody    body;
        HttpVersion version;
    };

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
        out << req.method << ' ' << req.uri
                          << ' ' << req.version << "\r\n";

        // Headers
        out << req.headers << "\r\n";

        //Body
        out << req.body;
        
        return out;
    }
} // namespace http
