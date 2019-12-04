#pragma once

#include <string>
#include <sstream>

#include "gtest/gtest.h"

template<typename Request>
void test_eq(Request const & req, std::string const & expected)
{
    std::ostringstream stream;
    stream << req;
    ASSERT_EQ(stream.str(), expected);
}
