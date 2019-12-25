#include "http/response.hpp"

#include "../misc.hpp"

TEST (BasicResponse, Default)
{
    http::http_response<> res;
    test_eq(res, "HTTP/1.1 200 OK\r\n\r\n");
}

TEST (BasicResponse, SetCodeEnum)
{
    http::http_response<> res;
    res._status_code = http::status_code_type::Bad_Request;
    test_eq(res, "HTTP/1.1 400 Bad Request\r\n\r\n");
}

TEST (BasicResponse, SetCodeString)
{
    http::http_response<> res;
    res._status_code = "Bad Request";
    test_eq(res, "HTTP/1.1 400 Bad Request\r\n\r\n");
}

TEST (BasicResponse, SetCodeInt)
{
    http::http_response<> res;
    res._status_code = 400;
    test_eq(res, "HTTP/1.1 400 Bad Request\r\n\r\n");
}

TEST (BasicResponse, WrongVersion)
{
    http::http_response<> res;
    res._version._version = "HTTP/1.10";
    test_eq(res, "");
    res._version._version = "";
    test_eq(res, "");
    res._version._version = "HTTP/11";
    test_eq(res, "");
    res._version._version = "1.1";
    test_eq(res, "");
}
