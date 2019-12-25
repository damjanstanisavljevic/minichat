#pragma once

#include "parser_helper.hpp"
#include "response.hpp"

namespace http
{
    namespace detail
    {
        http_status_code parse_status_code(std::string const & status_code)
        {
            if (status_code == "200 OK")
                return status_code_type::OK;
            else if (status_code == "400 Bad Request")
                return status_code_type::Bad_Request;
            std::string what("Unknown status code: " + status_code);
            throw std::invalid_argument(what + '\n');
        }

        std::vector<std::string> parse_status_line(std::string const & first_line)
        {
            auto delim = first_line.find_first_of(" ");
            if (delim == std::string::npos)
            {
                std::string what("Could not understand status line: " + first_line);
                throw std::invalid_argument(what + '\n');
            }

            return std::vector<std::string>{first_line.substr(0, delim),
                                            first_line.substr(delim + 1)};
        }
    } // namespace detail

    http_response<> parse_response(std::string message)
    {
        auto line = detail::get_line(message);
        auto status_line = detail::parse_status_line(line);

        http_headers headers;
            
        line = detail::get_line(message);
        while (line != "")
        {
            auto header = detail::parse_header(line);
            headers.add_header(header[0], header[1]);

            line = detail::get_line(message);
        }

        return http_response<>(detail::parse_status_code(status_line[1]),
                               detail::parse_version(status_line[0]),
                               std::move(headers),
                               detail::parse_body(message));
    }
} // namespcae http
