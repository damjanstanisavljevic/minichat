#pragma once

#include <type_traits>
#include <utility>

#include "../../include/json.hpp"
#include "headers.hpp"
#include "method.hpp"
#include "uri.hpp"
#include "version.hpp"

namespace http
{   
    template <typename HttpMethod  = http_method,
              typename HttpUri     = http_uri,
              typename HttpHeaders = http_headers,
              typename HttpBody    = nlohmann::json,
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
            return is_set_if_exists(method)
                && is_set_if_exists(uri)
                && is_set_if_exists(version)
                && is_set_if_exists(headers)
                && is_set_if_exists(body);
        }

        HttpMethod  method;
        HttpUri     uri;
        HttpHeaders headers;
        HttpBody    body;
        HttpVersion version;
    };

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
            return is_set_if_exists(o, has_is_set<typename std::decay_t<T>
                                                 >::type);
        }
    } // namespace detail

    template<typename OStream, typename HttpMethod,
                               typename HttpUri,
                               typename HttpBody,
                               typename HttpHeaders,
                               typename HttpVersion>
    OStream & operator<<(OStream & out, http_request<HttpMethod,
                                                     HttpUri,
                                                     HttpBody,
                                                     HttpHeaders,
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
