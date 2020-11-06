#ifndef __AND_TEST__
#define __AND_TEST__

#include "includes.hpp"
#include "gtest/gtest.h"

#include <fstream>


TEST(AndTest, LeftTrue)
{
    auto s = std::shared_ptr<OP_MOCK>(new OP_MOCK(10));
    auto f = std::shared_ptr<OP_MOCK>(new OP_MOCK(20));

    AndConnector andc {"unit_tests/TRUE", s, f};
    andc.execute();

    std::ifstream in;
    in.open("testfile");
    std::string line;
    in >> line;
    in.close();

    EXPECT_EQ(line, "10");
}

TEST(AndTest, LeftFalse)
{
    auto s = std::shared_ptr<OP_MOCK>(new OP_MOCK(10));
    auto f = std::shared_ptr<OP_MOCK>(new OP_MOCK(20));

    AndConnector andc {"unit_tests/FALSE", s, f};
    andc.execute();

    std::ifstream in;
    in.open("testfile");
    std::string line;
    in >> line;
    in.close();
    EXPECT_EQ(line, "20");
}

#endif
