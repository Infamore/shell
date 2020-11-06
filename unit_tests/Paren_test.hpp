#ifndef __PAREN_TEST__
#define __PAREN_TEST__

#include "includes.hpp"
#include "gtest/gtest.h"
#include <fstream>

TEST(ParenTest, Change_Order_Of_Ops)
{
    auto paren = std::shared_ptr<Element>(new Paren("unit_tests/print_test bar || unit_tests/print_test baz"));

    OrConnector orc {"unit_tests/print_test foo", nullptr, paren}; // nothing if success, execute parentheses if failure
    orc.execute();

    std::ifstream in;
    in.open("testfile");
    std::string line;
    in >> line;
    in.close();

    EXPECT_EQ(line, "foo");
}

#endif
