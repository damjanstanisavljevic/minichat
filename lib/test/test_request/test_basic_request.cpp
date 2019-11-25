#include <sstream>

#include "gtest/gtest.h"
#include "http/request.hpp"

TEST (ParseBasicRequest, Default)
{
    http::http_request<> req;
    req.uri.uri = "http://test.com/basic";
    std::ostringstream stream;
    stream << req;
    auto str = stream.str();

    ASSERT_STREQ(stream.str().c_str(), "GET http://test.com/basic HTTP/1.1\r\n\r\n");
}
