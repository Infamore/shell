#ifndef __TEST__
#define __TEST__

#include "element.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

class RTest : public Element
{
    private:
        std::vector<std::string> str_tok; 

    public:

        RTest(std::vector<std::string> & args)
        {
            str_tok = args;
        }

        ~RTest() {}

        virtual int execute()
        {   
            struct stat stats;

            int exists = (stat(str_tok[str_tok.size() - 1].c_str(), &stats) == 0);
   
            if (strcmp(str_tok[0].c_str(), "-d") == 0 && S_ISDIR(stats.st_mode) != 0)
            {
                std::cout << "(TRUE)" << std::endl;
                return 0;
            }		
            else if (strcmp(str_tok[0].c_str(), "-f") == 0 && S_ISREG(stats.st_mode) != 0)
            {
                std::cout << "(TRUE)" << std::endl;
                return 0;
            }
            else if (strcmp(str_tok[0].c_str(), "-e") == 0 && exists)
            {
                std::cout << "(TRUE)" << std::endl;
                return 0;
            }
            else if (str_tok.size() == 1 && exists)
            {
                std::cout << "(TRUE)" << std::endl;
                return 0;
            }
            else
            {
                std::cout << "(FALSE)" << std::endl;
                return 1;
            }
            std::cout << "(FALSE)" << std::endl;
            return 1;
        }
        virtual void print()
        {
            std::cout << "\033[34mTest: " << this << "\033[0m\n    Line: test ";
            for (auto s : str_tok)
            {
                std::cout << s << " ";
            }
            std::cout << std::endl;
        }

};

#endif // __TEST__
