#pragma once

#include <regex>
#include <string>

#include <boost/algorithm/string.hpp>

#include "body.hpp"
#include "headers.hpp"
#include "version.hpp"

namespace http
{
    namespace detail
    {
        inline std::string get_line(std::string & message)
        {
            auto line = message.substr(0, message.find_first_of("\r\n"));
            message.erase(0, line.size() + 2);
            return line;
        }

        inline http_body parse_body(std::string const & body)
        {
            if (body == "")
                return http_body();
            return http_body(body);
        }

        inline http_version parse_version(std::string const & version)
        {
            if (!std::regex_match(version,
                                  std::regex("(HTTP/)([0-9])(.)([0-9])")))
            {
                std::string what("Unknown version: " + version);
                throw std::invalid_argument(what + '\n');
            }

            return http_version(version);
        }

        inline std::vector<std::string> parse_header(std::string const & line)
        {

            auto delim = line.find_first_of(":");
            if (delim == std::string::npos)
            {
                std::string what("Could not understand header: " + line);
                throw std::invalid_argument(what + '\n');
            }

            auto header_name  = line.substr(0, delim);
            auto header_field = line.substr(delim + 1);

            boost::algorithm::trim(header_name);
            boost::algorithm::trim(header_field);

            boost::algorithm::to_lower(header_name);

            return std::vector<std::string>{header_name, header_field};
        }
    } // namespace detail
} // namespace http