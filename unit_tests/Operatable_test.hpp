#ifndef __OPERATABLE_TEST__
#define __OPERATABLE_TEST__

#include "includes.hpp"
#include "gtest/gtest.h"
#include <vector>

TEST(OperatableTest, ReturnTrue)
{
    std::vector<std::string> argv;
    argv.push_back("unit_tests/TRUE");
    
    Operatable op {argv};

    EXPECT_EQ(op.execute(), 0);
}

TEST(OperatableTest, ReturnFalse)
{
    std::vector<std::string> argv;
    argv.push_back("unit_tests/FALSE");
    
    Operatable op {argv};

    EXPECT_EQ(op.execute(), 1);
}

#endif
