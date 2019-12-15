#pragma once

#include <map>

namespace http
{
    class http_headers
    {
        // member variables
        std::map<std::string, std::string> _headers;

    public:
        
        // rule of five and default construcotr
        explicit http_headers() = default;
        explicit http_headers(http_headers const &) = default;
        explicit http_headers(http_headers && ) = default;

        http_headers& operator=(http_headers const &) = default;
        http_headers& operator=(http_headers &&) = default;

        // default virtual destructor in case of inheritance
        virtual ~http_headers() = default;

        // constructor that takes a anythig that can be used to construct _headres
        template<typename T,
                 typename = std::enable_if_t<
                                std::is_constructible_v<std::decay_t<decltype(_headers)>, T>> >
        explicit http_headers(T && t) : _headers{std::forward<T>(t)}
        {}

        // method for adding headers, creates copise of passed arguments
        http_headers & add_header(std::string const & name, std::string const & field)
        {
            _headers.try_emplace(name,field);
            return *this;
        }

        // method for adding headers, moves from the passed arguments
        http_headers & add_header(std::string && name, std::string && field)
        {
            _headers.try_emplace(std::move(name),std::move(field));
            return *this;
        }

        // decalration of firend function opreatro<< for printing all of the headres
        template<typename OStream>
        friend OStream & operator<<(OStream & out, const http_headers & headers);
        
    };

    // operator<< simply iterates trough all of the elements and prints them in "name : filed \r\n" format
    template<typename OStream>
    OStream & operator<<(OStream & out, http_headers const & headers)
    {
        for (auto const & [name, field] : headers._headers)
            out << name << ": " << field << "\r\n";
        return out;
    }
} // namespace http
