#ifndef __TEST_TEST__
#define __TEST_TEST__

#include "includes.hpp"
#include "gtest/gtest.h"
#include "../src/element.hpp"
#include "../src/Test.hpp"
#include <fstream>
#include <string>
#include <vector>

TEST(Test_Test, test_exist_reg_true)
{
	std::vector<std::string> args_commands;
    args_commands.push_back("-e");
    args_commands.push_back("unit_tests/true.txt");
  
    RTest test1  (args_commands);     // check for file
    EXPECT_EQ(test1.execute(), 0); // file should exist
}


TEST(Test_Test, test_exist_dir_true)
{
	std::vector<std::string> args_commands;
    args_commands.push_back("-e");
    args_commands.push_back("unit_tests/true_dir");
  
    RTest test1 (args_commands);     // check for file
    EXPECT_EQ(test1.execute(), 0); // directory should exist
}


TEST(Test_Test, test_exist_false)
{
	std::vector<std::string> args_commands;
    args_commands.push_back("-e");
    args_commands.push_back("unit_tests/false.txt"); // should not work
  
    RTest test1 (args_commands);     // check for file
    EXPECT_EQ(test1.execute(), 1); // file should not exist
}


TEST(Test_Test, test_dir_true)
{
	std::vector<std::string> args_commands;
    args_commands.push_back("-d");
    args_commands.push_back("unit_tests/true_dir");
  
    RTest test1 (args_commands);     // check for file
    EXPECT_EQ(test1.execute(), 0); // file should exist
}


TEST(Test_Test, test_dir_false)
{
	std::vector<std::string> args_commands;
    args_commands.push_back("-d");
    args_commands.push_back("unit_tests/true.txt"); // file should not work
  
    RTest test1 (args_commands);     // check for file
    EXPECT_EQ(test1.execute(), 1); // file should not exist
}


TEST(Test_Test, test_reg_true)
{
	std::vector<std::string> args_commands;
    args_commands.push_back("-f");
    args_commands.push_back("unit_tests/true.txt");
  
    RTest test1 (args_commands);     // check for file
    EXPECT_EQ(test1.execute(), 0); // file should exist
}


TEST(Test_Test, test_reg_false)
{
	std::vector<std::string> args_commands;
    args_commands.push_back("-f");
    args_commands.push_back("unit_tests/true_dir"); // directory should not work
  
    RTest test1 (args_commands);     // check for file
    EXPECT_EQ(test1.execute(), 1); // file should not exist
}

#endif
