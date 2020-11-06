#ifndef __OUTPUT_OVERWRITE__
#define __OUTPUT_OVERWRITE__

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

#include "element.hpp"

//#define DEBUG
//#define VERBOSE
//#define DEBUG_DESTUCT

extern std::shared_ptr<Element> parse(std::string line, int * input, int * output);


class OutputOverwrite : public Element
{
private:
    std::shared_ptr<Element> left;
    std::string output;
    int fd[2];
public:
    OutputOverwrite(std::string args, int * input = nullptr)
    {
        std::string command;
        for(int i = 0; i < args.length(); i++)
        {
            if (args[i] == '>')
            {
                command = args.substr(0, i - 1);
                output = args.substr(i + 2);
                break;
            }
        }
        fd[1] = open(output.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0640);
        left = parse(command, input, fd);
    }
    ~OutputOverwrite()
    {
#ifdef DEBUG_DESTRUCT
        std::cout << "Deconstructing: " << this << std::endl;
#endif
        left.reset();
    }

    virtual int execute()
    {
        return left->execute();
    }

    virtual void print()
    {
        std::cout << "\033[33mOutputOverwrite: " << this << "\033[0m" << std::endl;
        std::cout << "    Left: " << left << std::endl;
        std::cout << "    output: " << output << " (" << fd[1] << ")" << std::endl;
        left->print();
    }
};

#endif // __OUTPUT_OVERWRITE__
