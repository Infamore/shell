#ifndef __OUTPUTAPPEND_TEST__
#define __OUTPUTAPPEND_TEST__

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
#include "OutputAppend.hpp"
#include "element.hpp"
#include "gtest/gtest.h"


TEST(OutputAppend, outputfile)
{
  //creates an empty file called outputfile.txt and adds the text "text from outputappend"
  
  bool file_exist = false;
	std::string filename = "outputfile.txt";
  std::string text = "text from outputappend";
	std::vector<std::string> outputvect;
  outputvect.push_back(text);
  outputvect.push_back(">>");
  outputvect.push_back(filename);
  //the token commands if rolled out becomes 'text from outputappend >> outputfile.txt'
  OutputRedirect* OR = new OuputRedirect(outputvect);
  int var_ = IR->execute(); //if there is an error with the syscall maybe tokenize the text string 
  
  std::fstream ofile;
  ofile.open(outputfile.txt, ios::out); //make sure that it opens an already existing file and does not create an arbitrary file with the same name, if an error occurs then try ios::in, or only have one parameter
  
  std::string holdstr;
  
  while(!ofile.eof())
    ofile >> holdstr;
  
  std::cout << holdstr << std::endl; //checks to see if text formatting is proper and if that's an issue with the string
  
  EXPECT_EQ("text from outputappend", holdstr);
  
}
#endif
