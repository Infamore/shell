#ifndef __INPUT_REDIRECT__
#define __INPUT_REDIRECT__

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

class InputRedirect : public Element
{
private:
    char ** args;
    std::string input;
    int in_fd, output[2];
    
public:
    InputRedirect(std::vector<std::string> _args, int * output = nullptr)
    {
#ifdef VERBOSE
        std::cout << "Addr: " << this << " Command: ";
        for (auto s : _args)
            std::cout << s << "-";
        std::cout << std::endl;
#endif

        if (output)
        {
            this->output[0] = output[0];
            this->output[1] = output[1];
        }
        else
        {
            this->output[0] = -1;
            this->output[1] = -1;
        }

        // create array of args
        args = (char **)calloc(_args.size() + 1, sizeof(char *));
        for(int i = 0; i < _args.size(); i++)
        {
            if (_args[i] != "<")
            {
                args[i] = (char *)calloc(_args[i].length() + 1, sizeof(char));
                strcpy(args[i], _args[i].c_str());
            }
            else
            {
                i++;
                input = _args[i];
            }
#ifdef DEBUG
            std::cout << "Adding " << args[i] << " to array" << std::endl;
#endif
        }
    }
    ~InputRedirect()
    {
#ifdef DEBUG_DESTRUCT
        std::cout << "Deconstructing: " << this << std::endl;
#endif
        int i = 0;
        while(args[i])
        {
            free(args[i++]);
        }
        free(args);
    }

    virtual int execute()
    {
#ifdef DEBUG
        int i = 0;
        while(args[i])
        {
            std::cout << args[i++] << " ";
        }
        std::cout << std::endl;
#endif
        /*
         *fork and execute here
         */
        int pid;
        int stat;

        pid = fork();

        if (pid > 0) // parent
                {
                    if (waitpid(pid, &stat, 0) == -1)
                    {
                        perror("There was an error");
                        exit(EXIT_FAILURE);
                    }
                    if (stat == 0)
                    {
#ifdef DEBUG
                    std::cout << "waitpid succeeded" << std::endl;
#endif
                        return 0;
                    }
                    else
                        return 1;
        }

        if (pid == 0)
        {
            // swap output files
            in_fd = open(input.c_str(), O_RDONLY, 644);
            if (in_fd < 0)
            {
                perror("Error opening output file.");
                exit(-1);
            }
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);

            if (output[0] != -1)
            {
                dup2(output[1], STDOUT_FILENO);
                close(output[1]);
            }

            // execute
            execvp(args[0], args);
            
                    perror("error");
            exit(-1);
        }

        if (pid == -1)
        {
            perror("child process not created");
            exit(-1);
        }
        return 1;
    }

    virtual void print()
    {
        std::cout << "\033[33mInputRedirect: " << this << "\033[0mLine: " << std::endl;
        int i = 0;
        while (args[i] != 0)
        {
            std::cout << args[i++] << " ";
        }
        std::cout << "Input:  " << input << std::endl;
        std::cout << "Output: " << output[1] << std::endl;
    }
    
    int get_fd() {
        return in_fd;
    }
};

#endif // __INPUT_REDIRECT__
