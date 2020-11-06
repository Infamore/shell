#ifndef __OPERATABLE__
#define __OPERATABLE__

#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "element.hpp"

//#define DEBUG
//#define VERBOSE
//#define DEBUG_DESTUCT

class Operatable : public Element
{
private:
    char ** args;
    int input[2], output[2];
public:
    Operatable(std::vector<std::string> _args, int * input = nullptr, int * output = nullptr)
    {
        if (input)
        {
            this->input[0] = input[0];
            this->input[1] = input[1];
        }
        else
        {
            this->input[0] =-1;
            this->input[1] =-1;
        }
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
#ifdef VERBOSE
        std::cout << "Addr: " << this << " Command: ";
        for (auto s : _args)
            std::cout << s << "-";
        std::cout << std::endl;
#endif
        // create array of args
        args = (char **)calloc(_args.size() + 1, sizeof(char *));
        for(int i = 0; i < _args.size(); i++)
        {
            if (_args[i][0] == '"' && _args[i][_args[i].length() - 1] == '"')
                _args[i] = _args[i].substr(1, _args[i].length() - 2);
            //std::cout << _args[i] << std::endl;
            args[i] = (char *)calloc(_args[i].length() + 1, sizeof(char));
            strcpy(args[i], _args[i].c_str());
#ifdef DEBUG
            std::cout << "Adding " << args[i] << " to array" << std::endl;
#endif
        }
    }
    ~Operatable()
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
        std::cout << ":" << std::endl;
#endif
        /*
         *fork and execute here
         */
        int pid;
        int stat;

        pid = fork();

        if (pid > 0) // parent
            {
                close(input[1]); // close parent input write
                close(input[0]); // close parent input read
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

        if (pid == 0) // child
        {
            if (input[0] != -1)
            {
                close(input[1]); // close child input write
                //std::cout << "Swapping in" << std::endl;
                dup2(input[0], STDIN_FILENO);
                close(input[0]); // close child input read
            }
            if (output[0] != -1)
            {
                //std::cout << "Swapping out" << std::endl;
                dup2(output[1], STDOUT_FILENO);
                close(output[1]); // close child output write
                //close(output[0]); // close child output read
            }

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
        // demo code
        /*
        std::cout << "Executing ";
        int i = 0;
        while (args[i])
        {
            std::cout << args[i++] << " ";
        }
        std::cout << std::endl;

        if (strcmp(args[0], "1") == 0)
            return 1;

        return 0; // returns success for now
        */
    }

    virtual void print()
    {
        std::cout << "\033[33mCommand: " << this << "\033[0m\n    Line: ";
        int i = 0;
        while (args[i] != 0)
        {
            std::cout << args[i++] << " ";
        }
        std::cout << std::endl;
        std::cout << "    Input from: " << input[0] << std::endl;
        std::cout << "    Output to:  " << output[1] << std::endl;
    }
};

#endif // __OPERATABLE__
