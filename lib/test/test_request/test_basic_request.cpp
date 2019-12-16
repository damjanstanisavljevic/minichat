#include "http/request.hpp"

#include "../misc.hpp"

TEST (BasicRequest, Default)
{
    http::http_request<> req;
    req._uri._uri = "http://test.com/basic";
    test_eq(req, "GET http://test.com/basic HTTP/1.1\r\n\r\n");
}

TEST (BasicRequest, ReqNotSet)
{
    http::http_request<> req;
    test_eq(req, "");
}

TEST (BasicRequest, SetMethodEnum)
{
    http::http_request<> req;
    req._method = http::method_type::DELETE;
    req._uri._uri = "http://test.com/basic";
    test_eq(req, "DELETE http://test.com/basic HTTP/1.1\r\n\r\n");
}

TEST (BasicRequest, SetMethodString)
{
    http::http_request<> req;
    req._method = "OPTIONS";
    req._uri._uri = "http://test.com/basic";
    test_eq(req, "OPTIONS http://test.com/basic HTTP/1.1\r\n\r\n");
}

TEST (BasicRequest, WrongVersion)
{
    http::http_request<> req;
    req._version._version = "HTTP/1.10";
    test_eq(req, "");
    req._version._version = "";
    test_eq(req, "");
    req._version._version = "HTTP/11";
    test_eq(req, "");
    req._version._version = "1.1";
    test_eq(req, "");
}
