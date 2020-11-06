#ifndef __PIPE_TEST__
#define __PIPE_TEST__
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
#include "Pipe.hpp"
#include "element.hpp"
#include "gtest/gtest.h"


TEST(Piping, pipefile)
{
  //creates an empty file called outputfile.txt and adds the text "text from outputappend"
  
  bool file_exist = false;
	/*
	std::string filename = "pipefile.txt";
  std::string text = "grep random"; 
	std::vector<std::string> outputvect;
  outputvect.push_back("cat")
  outputvect.push_back(filename);
  outputvect.push_back("|");
  outputvect.push_back(text);
  */
	std::string filename = "pipefile.txt";
  std::string pipecommand = "cat pipefile.txt | grep random";
  
	ofstream ofile;
  ofile.open(filename, std::fstream::out | std::fstream::app);
  ofile << "random words";
  ofile.close();
  
  Pipe* p = new Pipe(pipecommand);
  int var_ = p->execute(); //if there is an error with the syscall maybe tokenize the text string 
  
  bool no_errors = true;
  
  if (var_ != 0) //execute function returns a 0 if no errors
		    no_errors = false;
  
  EXPECT_TRUE(no_errors);
  
}
 #endif
