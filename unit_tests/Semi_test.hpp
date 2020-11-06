#ifndef __SEMI_TEST__
#define __SEMI_TEST__

#include "includes.hpp"
#include "gtest/gtest.h"
#include <fstream>

TEST(SemiTest, LeftTrue)
{
    auto s = std::shared_ptr<OP_MOCK>(new OP_MOCK(10));

    SemiConnector semic {"unit_tests/TRUE", s};
    semic.execute();

    std::ifstream in;
    in.open("testfile");
    std::string line;
    in >> line;
    in.close();

    EXPECT_EQ(line, "10");
}

TEST(SemiTest, LeftFalse)
{
    auto s = std::shared_ptr<OP_MOCK>(new OP_MOCK(10));

    SemiConnector semic {"unit_tests/FALSE", s};
    semic.execute();

    std::ifstream in;
    in.open("testfile");
    std::string line;
    in >> line;
    in.close();

    EXPECT_EQ(line, "10");
}

#endif
