#include "http/request_parser.hpp"

#include "../test_utility.hpp"

TEST (ParserRequestGood, Basic)
{
    std::string req_str = "GET / HTTP/1.1\r\n\r\n";
    auto req_obj = http::parse_request(req_str);
    test_eq(req_obj, req_str);
}

TEST (ParserRequestGood, OneHeader)
{
    std::string req_str =
        "GET / HTTP/1.1\r\ncontent-type: application/json\r\n\r\n";
    auto req_obj = http::parse_request(req_str);
    test_eq(req_obj, req_str);
}

TEST (ParserRequestGood, MultipleHeaders)
{
    std::string req_str =
        "GET / HTTP/1.1\r\ncontent-type: html\r\nhost: 127.0.0.1:42042\r\n\r\n";
    auto req_obj = http::parse_request(req_str);
    test_eq(req_obj, req_str);
}

TEST (ParserRequestGood, WithBody)
{
    std::string req_str =
        "POST /test HTTP/1.0\r\ncontent-length: 24\r\n\r\n{\"test\":\"parserRequest\"}";
    auto req_obj = http::parse_request(req_str);
    test_eq(req_obj, req_str);
}

TEST (ParserRequestFail, UnkownMethod)
{
    test_exception(http::parse_request,
                   "GER / HTTP/1.0\r\n\r\n",
                   "Unknown method: GER\n");
}

TEST (ParserRequestFail, UnknownVersion)
{
    test_exception(http::parse_request,
                   "GET / HTTP/11\r\n\r\n",
                   "Unknown version: HTTP/11\n");
}

TEST (ParserRequestFail, ShortRequestLine)
{
    test_exception(http::parse_request,
                   "GET /\r\n\r\n",
                   "The request line is too short.\n");
}

TEST (ParserRequestFail, LongRequestLine)
{
    test_exception(http::parse_request,
                   "GET / HTTP/1.1 header:\r\n\r\n",
                   "The request line is too long.\n");
}

TEST (ParserRequestFail, WrongHeader)
{
    test_exception(http::parse_request,
                   "GET / HTTP/1.1\r\nheader = wrong\r\n\r\n",
                   "Could not understand header: header = wrong\n");
}
