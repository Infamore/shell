#ifndef __PAREN__
#define __PAREN__


#include <iostream>
#include "element.hpp"


std::shared_ptr<Element> parse(std::string line, int * input, int * output);

class Paren : public Element
{
    private:
        std::shared_ptr<Element> child;
        int input[2], output[2];
    public:
        Paren(std::string commands, int * input = nullptr, int * output = nullptr)
        {
            if (input)
            {
                this->input[0] = input[0];
                this->input[1] = input[1];
            }
            else
                this->input[0] = -1;
            if (output)
            {
                this->output[0] = output[0];
                this->output[1] = output[1];
            }
            else
                this->output[0] = -1;

            child = parse(commands, input, output);
        }

        ~Paren()
        {
            child.reset(); 
        }

        virtual int execute()
        {
            return child->execute();
        }
        virtual void print()
        {
            std::cout << "\033[35;1mParentheses: " << this << "\033[0m\n    Child: " << child << std::endl;
            child->print();
        }
};
#endif // __PAREN__
