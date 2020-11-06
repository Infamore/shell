#ifndef __INPUTREDIRECT_TEST__
#define __INPUTREDIRECT_TEST__

#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include "operatable.hpp"
#include "InputRedirect.hpp"
#include "element.hpp"
#include "gtest/gtest.h"


TEST(InputRedirect, inputfile)
{
  bool file_exist = false;
  std::string command = "cat";
	std::string filename = "randomfile.txt";
	std::vector<std::string> inputvect;
  inputvect.push_back(commmand);
  inputvect.push_back("<");
  inputvect.push_back(filename);
  
  ofstream ofile;
  ofile.open(filename, std::fstream::out | std::fstream::app);
  ofile << "nothing here";
  ofile.close();
  
  InputRedirect* IR = new InputRedirect(inputvect);
  int var_ = IR->execute();
  
  EXPECT_TRUE(file_exist);
  EXPECT_EQ(IR->get_fd(), 644); 

}
#endif
