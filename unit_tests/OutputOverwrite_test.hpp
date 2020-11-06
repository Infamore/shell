#ifndef __OUTPUTOVERWRITE_TEST__
#define __OUTPUTOVERWRITE_TEST__

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
#include <sstream>
#include "operatable.hpp"
#include "OutputOverwrite.hpp"
#include "element.hpp"
#include "gtest/gtest.h"


TEST(OutputOverwrite, file)
{
  //creates an empty file called outputfile.txt and adds the text "text from outputappend"
  
  bool file_exist = false;
	std::string filename = "outputrandomfile.txt";
  std::string text = "overwritten by outputoverwrite class";
	std::vector<std::string> outputvect;
  outputvect.push_back(text);
  outputvect.push_back(">");
  outputvect.push_back(filename);
  //the token commands if rolled out becomes 'overwritten by outputoverwrite class > outputrandomfile.txt'
  
	ofstream ofile;
  ofile.open(filename, std::fstream::out | std::fstream::app);
  ofile << "nothing here";
  ofile.close();
  
  OutputRedirect* OWR = new OuputOverwrite(outputvect);
  int var_ = OWR->execute(); //if there is an error with the syscall maybe tokenize the text string 
  
  std::string holdstr;
  
  while(!ofile.eof())
    ofile >> holdstr;
  
  std::cout << holdstr << std::endl; //checks to see if text formatting is proper and if that's an issue with the string
  
  EXPECT_EQ("overwritten by outputoverwrite class", holdstr);
  
}
#endif
