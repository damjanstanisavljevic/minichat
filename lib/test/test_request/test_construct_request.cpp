#include "http/request.hpp"

#include "../test_utility.hpp"

TEST (ConstructRequest, Default)
{
    http::http_request<> req(http::method_type::GET,
                             http::http_uri("http://test.com/basic"),
                             http::http_headers(),
                             http::http_body(),
                             http::http_version());
    test_eq(req, "GET http://test.com/basic HTTP/1.1\r\n\r\n");
}

TEST (ConstructRequest, NotCopy)
{
    http::http_request<> req(http::method_type::GET,
                             "http://test.com/basic");
    test_eq(req, "GET http://test.com/basic HTTP/1.1\r\n\r\n");
}

TEST (ConstructRequest, ReqNotSet)
{
    http::http_request<> req(http::method_type::GET,
                             http::http_uri(),
                             http::http_headers(),
                             http::http_body(),
                             http::http_version());
    test_eq(req, "");
}
