#pragma once

#include "parser_helper.hpp"
#include "request.hpp"

namespace http
{
    namespace detail
    {
        http_method parse_method(std::string const & method)
        {
            if (method == "DELETE")
                return method_type::DELETE;
            else if (method == "GET")
                return method_type::GET;
            else if (method == "OPTIONS")
                return method_type::OPTIONS;
            else if (method == "PATCH")
                return method_type::PATCH;
            else if (method == "POST")
                return method_type::POST;
            else if (method == "PUT")
                return method_type::PUT;
            std::string what("Unknown method: " + method);
            throw std::invalid_argument(what + '\n');
        }

        std::vector<std::string> parse_request_line(std::string const & first_line)
        {
            std::vector<std::string> request_line;
            boost::algorithm::split(request_line, first_line,
                                    boost::algorithm::is_space());
            
            if (request_line.size() < 3)
                throw std::invalid_argument("The request line is too short.\n");
            else if (request_line.size() > 3)
                throw std::invalid_argument("The request line is too long.\n");
            
            return request_line;
        }
    } // namespace detail

    http_request<> parse_request(std::string message)
    {
        auto line = detail::get_line(message);
        auto request_line = detail::parse_request_line(line);

        http_headers headers;
            
        line = detail::get_line(message);
        while (line != "")
        {
            auto header = detail::parse_header(line);
            headers.add_header(header[0], header[1]);

            line = detail::get_line(message);
        }

        return http_request<>(detail::parse_method(request_line[0]),
                              http_uri(request_line[1]),
                              std::move(headers),
                              detail::parse_body(message),
                              detail::parse_version(request_line[2]));
    }
} // namespcae http
