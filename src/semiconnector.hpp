#ifndef __SEMI_CONNECTOR__
#define __SEMI_CONNECTOR__

#include "BaseConnector.hpp"
#include "Operatable.hpp"
#include "OutputOverwrite.hpp"
#include "OutputAppend.hpp"
#include "InputRedirect.hpp"
#include "Paren.hpp"
#include "Pipe.hpp"
#include "Test.hpp"
#include <vector>
#include <string>

//#define DEBUG
//#define VERBOSE

extern std::shared_ptr<Element> parse(std::string line, int * input, int * output);

class SemiConnector : public BaseConnector
{
private:
    std::vector<std::string> parse(std::string args)
    {
        std::vector<std::string> v;
        int s = 0;
        int itr = 0;
        while(itr < args.length())
        {
            if (args[itr] == ' ')
            {
#ifdef DEBUG
                std::cout << args.substr(s, itr - s) << std::endl;
#endif
                v.push_back(args.substr(s, itr - s));
                s = itr + 1;
            }
            itr++;
        }
        v.push_back(args.substr(s));
        return v;
    }

public:
    SemiConnector(std::string command, std::shared_ptr<Element> next, int * input = nullptr, int * output = nullptr)
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

        for (int i = command.length() - 1; i > -1; i++)
        {
            if (command[i] == '|')
                left = new Pipe(command, output);
        }
        if (command[0] == '(') // construct parentheses 
        {
            chomp(command);
            command = command.substr(1, command.length() - 2); // remove surrounding (  )
            this->left = new Paren(command, input, output);
        }
        else
        {
            std::vector<std::string> args = this->parse(command);

            if (args[0] == "test" || args[0] == "[") // is a test
            {
                args.erase(args.begin());
                if (*(args.end() - 1) == "]")
                {
                    args.erase(args.end() - 1);
                }
                this->left = new RTest(args);
            }
            else
                this->left = parse(command, input, output);
        }
        this->success = next;
        this->failure = next;
#ifdef VERBOSE
        std::cout << "Addr: " << this << " Left: " << left << " Next: " << next << std::endl;
#endif
    }

    ~SemiConnector() {}

    virtual void print()
    {
        std::cout << "\033[36mSemi Connector: " << this << "\033[0m\n    Left: " << left << "\n    Next: " << success << std::endl;
        left->print();
        if (success) success->print();
    }
};

#endif // __SEMI_CONNECTOR__
