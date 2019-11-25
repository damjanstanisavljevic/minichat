#pragma once

#include "../../include/json.hpp"

namespace http
{
    struct http_body
    {
        http_body() = default;

        nlohmann::json json;
    };

    template<typename OStream>
    OStream & operator<<(OStream & out, http_body body)
    {
        if (!body.json.empty())
            out << body.json;
        return out;
    }
} // namespace http
