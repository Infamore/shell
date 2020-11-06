#ifndef __OR_TEST__
#define __OR_TEST__

#include "includes.hpp"
#include "gtest/gtest.h"
#include <fstream>

TEST(OrTest, LeftTrue)
{
    auto s = std::shared_ptr<OP_MOCK>(new OP_MOCK(10));
    auto f = std::shared_ptr<OP_MOCK>(new OP_MOCK(20));

    OrConnector orc {"unit_tests/TRUE", s, f};
    orc.execute();

    std::ifstream in;
    in.open("testfile");
    std::string line;
    in >> line;
    in.close();

    EXPECT_EQ(line, "10");
}

TEST(OrTest, LeftFalse)
{
    auto s = std::shared_ptr<OP_MOCK>(new OP_MOCK(10));
    auto f = std::shared_ptr<OP_MOCK>(new OP_MOCK(20));

    OrConnector orc {"unit_tests/FALSE", s, f};
    orc.execute();

    std::ifstream in;
    in.open("testfile");
    std::string line;
    in >> line;
    in.close();
    EXPECT_EQ(line, "20");
}

#endif
