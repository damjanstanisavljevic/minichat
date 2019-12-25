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

template <typename Parser>
void test_exception(Parser parser,
                    std::string const & obj,
                    std::string const & expected)
{
    try {
        parser(obj);
        FAIL() << "Expected std::invalid_argument";
    } catch (std::exception const & e) {
        ASSERT_EQ(e.what(), expected);
    } catch (...) {
        FAIL() << "Expected std::invalid_argument";
    }
}
