#include "element.hpp"
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include <iostream>

class Operaterable : public Element
{
	
	
private:
	std::vector<std::string> argv;
	pid_t pid;
	int stat;
public:
	Operaterable(std::vector<std::string> vect) : argv(vect) {}; // make this parse the string


	bool execute() {
                pid = fork();
                if (pid == 1) {
                        //std::cout << "pid = 1" << std::endl;
                        if (waitpid(pid, &stat, WCONTINUED) < 0)
                        {
                                perror("error");
                                exit(-1);
                        }
                        if (stat == 0) {
                                std::cout << "waitpid succeeded" << std::endl;
                                return true;
                        }
                        else
                                return false;
                }

                if (pid == 0) {
                        //std::cout << "pid = 0" << std::endl;

                        std::vector<char*> argc;
                        int vec_size = argv.size();
  			for (int i = 0; i < vec_size; i++)
                                argc.push_back(const_cast<char*>(argv[i].c_str()));
                        argc.push_back(NULL);
                        char *args[] = {};
                        int argcsize = argc.size();
                        for(int i = 0; i < argcsize; i++) {
                                args[i] = argc[i];
                        
                        }
                   
                        if (execvp(args[0], &args[0]) == -1)
                        {
                                perror("execvp failed");
                                exit(-1);
                        }
            
                        return true; //execvp() did not have any errors
                }


		if (pid == -1)
		{
		//std::cout << "pid = -1" << std::endl;
			perror("child process not created");
			exit(-1);
		}
		return false;
	}
};
