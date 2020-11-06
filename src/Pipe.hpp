#ifndef __PIPE__
#define __PIPE__


#include <iostream>
#include <unistd.h>
#include "element.hpp"

std::shared_ptr<Element> parse(std::string line, int * input, int * output);

class Pipe : public Element
{
    private:
        std::shared_ptr<Element> left, right;
        int pipe_ends[2], input[2], output[2];
    public:
        Pipe(std::string commands, int * input = nullptr, int * output = nullptr)
        {
            if (input)
            {
                this->input[0] = input[0];
                this->input[1] = input[1];
            }
            else
            {
                this->input[0] = -1;
                this->input[1] = -1;
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

            int sep = -1;
            for (int i = commands.length() - 1; i > -1; i--)
            {
                if (commands[i] == '|')
                {
                    sep = i;
                    break;
                }
            }
            
            //i'm not sure what input and output pointers do in the parameters. I did not include *input or *output for avoiding potential
            //compiler errors with the input/output arrays
            //std::cout << "before parse" << std::endl;
            //std::cout << "input[0]: " << input[0] << "\n";
            //std::cout << "input[1]: " << input[1] << "\n";
            //std::cout << "output[0]: " << output[0] << "\n";
            //std::cout << "output[1]: " << output[1] << "\n";
            //std::cout << "pipe_ends[0]: " << pipe_ends[0] << "\n";
            //std::cout << "pipe_ends[1]: " << pipe_ends[1] << "\n";
            
            
            
            
            pipe(pipe_ends);
            /*
            if (this->input[0] != -1)
                left = parse(commands.substr(0, sep - 1), this->input, pipe_ends);
            else
                left = parse(commands.substr(0, sep - 1), nullptr, pipe_ends);
            if (this->output[0] != -1)
                right = parse(commands.substr(sep + 2), pipe_ends, this->output);
            else
                right = parse(commands.substr(sep + 2), pipe_ends, nullptr);
            */
            left = parse(commands.substr(0, sep - 1), input, pipe_ends);
            right = parse(commands.substr(sep + 2), pipe_ends, output);
            
            //std::cout << "after parse" << std::endl;
            //std::cout << "input[0]: " << input[0] << "\n";
            //std::cout << "input[1]: " << input[1] << "\n";
            //std::cout << "output[0]: " << output[0] << "\n";
            //std::cout << "output[1]: " << output[1] << "\n";
            //std::cout << "pipe_ends[0]: " << pipe_ends[0] << "\n";
            //std::cout << "pipe_ends[1]: " << pipe_ends[1] << "\n";
        }

        ~Pipe()
        {
            left.reset();
            right.reset();
        }

        virtual int execute()
        {
            int stat = -999;

            stat = left->execute();
            
            //close(pipe_ends[1]);
            //close(input[0]);
            //close(input[1]);
            if (stat == 0)
            {
                stat = right->execute();
            }
            //close(pipe_ends[0]);
            //close(output[1]);
            //close(output[0]);
            return stat;
        }
        virtual void print()
        {
            std::cout << "\033[35;1mPipe: " << this << "\033[0m\n    Left: " << left << "\n    Right: " << right << std::endl;
            std::cout << "    Pipe In:  " << pipe_ends[1] << std::endl;
            std::cout << "    Pipe Out: " << pipe_ends[0] << std::endl;
            std::cout << "    Input:    " << input[0] << std::endl;
            std::cout << "    Output:   " << output[1] << std::endl;
            left->print();
            right->print();
        }
};
#endif // __PIPE__
