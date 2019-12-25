#include "http/response.hpp"

#include "../test_utility.hpp"

TEST (ConstructResponse, Default)
{
    http::http_response<> res(http::status_code_type::OK,
                              http::http_version(),
                              http::http_headers(),
                              http::http_body());
    test_eq(res, "HTTP/1.1 200 OK\r\n\r\n");
}

TEST (ConstructResponse, NotCopy)
{
    http::http_response<> res(400);
    test_eq(res, "HTTP/1.1 400 Bad Request\r\n\r\n");
}

TEST (ConstructResponse, ReqNotSet)
{
    http::http_response<> res(http::status_code_type::OK,
                              http::http_version(""),
                              http::http_headers(),
                              http::http_body());
    test_eq(res, "");
}
