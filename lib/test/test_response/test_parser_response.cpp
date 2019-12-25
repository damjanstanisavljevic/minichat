#include "http/response_parser.hpp"

#include "../test_utility.hpp"

TEST (ParserResponseGood, Basic)
{
    std::string res_str = "HTTP/1.1 200 OK\r\n\r\n";
    auto res_obj = http::parse_response(res_str);
    test_eq(res_obj, res_str);
}

TEST (ParserResponseGood, OneHeader)
{
    std::string res_str =
        "HTTP/1.1 200 OK\r\ncontent-type: application/json\r\n\r\n";
    auto res_obj = http::parse_response(res_str);
    test_eq(res_obj, res_str);
}

TEST (ParserResponseGood, MultipleHeaders)
{
    std::string res_str =
        "HTTP/1.1 200 OK\r\ncontent-type: html\r\ndate: Wed, 25 Dec 2019 15:15:15 GMT\r\n\r\n";
    auto res_obj = http::parse_response(res_str);
    test_eq(res_obj, res_str);
}

TEST (ParserResponseGood, WithBody)
{
    std::string res_str =
        "HTTP/1.0 400 Bad Request\r\ncontent-length: 25\r\n\r\n{\"test\":\"parserResponse\"}";
    auto res_obj = http::parse_response(res_str);
    test_eq(res_obj, res_str);
}

TEST (ParserResponseFail, UnkownStatusCode)
{
    test_exception(http::parse_response,
                   "HTTP/1.0 666 Hell\r\n\r\n",
                   "Unknown status code: 666 Hell\n");
}

TEST (ParserResponseFail, UnknownVersion)
{
    test_exception(http::parse_response,
                   "HTTP/11 400 Bad Request\r\n\r\n",
                   "Unknown version: HTTP/11\n");
}

TEST (ParserResponseFail, CouldNotUnderstand)
{
    test_exception(http::parse_response,
                   "OK\r\n\r\n",
                   "Could not understand status line: OK\n");
}

TEST (ParserResponseFail, WrongHeader)
{
    test_exception(http::parse_response,
                   "GET / HTTP/1.1\r\nheader = wrong\r\n\r\n",
                   "Could not understand header: header = wrong\n");
}
