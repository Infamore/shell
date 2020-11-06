
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

#include "element.hpp"
#include "andconnector.hpp"
#include "orconnector.hpp"
#include "semiconnector.hpp"
#include "Operatable.hpp"
#include "Paren.hpp"
#include "Test.hpp"
#include "OutputOverwrite.hpp"
#include "OutputAppend.hpp"
#include "InputRedirect.hpp"

#define DEBUG_MAIN      // make compilable
#define DEBUG_STRINGS   // print strings of parsed line
#define DEBUG_CREATION  // states when elements are being constructed
//#define DEBUG_PARSE     // indicates current char at itr, prints 


// remove leading and trailing whitespace
void chomp(std::string & s)
{
    auto f_itr = s.begin();
    auto b_itr = s.end() - 1;
    while (*f_itr == ' ')
    {
        f_itr++;
    }
    while (*b_itr == ' ')
    {
        b_itr--;
    }
    s = std::string(f_itr, b_itr + 1);
}

std::shared_ptr<Element> parse(std::string line, int * input = nullptr, int * output = nullptr)
{   
    /* -----------------------------------------------------------
     * -- Parse command into strings of "[command] [connector]" --
     * ----------------------------------------------------------- */

    std::vector<std::string> tokens;

    int itr = 0;     // walk line
    int start = 0;   // start postion of chunk
    int closed;      // for skipping quote chunks and ()
    int paren_count; // for counting nested parentheses
    while(itr <= line.length())
    {
#ifdef DEBUG_PARSE
        std::cout << line << std::endl;
        for (int i = 0; i < itr; i++)
        {
            std::cout << " ";
        }
        std::cout << "^\n" << std::endl;
#endif
        // find && or ||
        if ((line[itr - 1] == '&' && line[itr] == '&') || ((line[itr - 1] == '|' && line[itr] == '|')))
        {
            tokens.push_back(line.substr(start, itr + 1 - start));
            start = itr + 1;
            itr = start;
        }
        // find ;
        else if (line[itr] == ';') 
        {
            tokens.push_back(line.substr(start, itr + 1 - start));
            start = itr + 1;
            itr = start;
        }
        // find " and skip to end or give error
        else if (line[itr] == '"')
        {   
            closed = 0;
            while (!closed)
            {
                itr = line.find('\"', itr + 1); // move itr forward to next "
                if (itr == std::string::npos)   // could not find last "
                {
                    perror("Error: Reached end of command without finding terminal '\"'");
                    return nullptr;
                }
                if (line[itr -1] != '\\')   // check if terminal " not \"
                {
                    closed = 1;
                }
            }
            itr++;
        }
        // find ( and skip to matching )
        else if (line[itr] == '(')
        {
            closed = 0;
            paren_count = 1;
            itr++;
            while(!closed)
            {
                if (line[itr] == '(')
                {
                    paren_count++;
                }
                if (line[itr] == ')')
                {
                    paren_count--;
                    if (paren_count == 0) // found matching parenthesis
                    {
                        closed = 1;
                    }
                }
                if (itr == line.length()) // ran out of characters
                {
                    perror("Error: Reached end of command without finding matching ')'");
                    return nullptr;
                }
                itr++;
            }
        }
        // add last token to line (found end of string)
        else if (itr >= line.length() && start < line.length() - 1)
        {
            tokens.push_back(line.substr(start));
            itr++;
        }
        // nothing at line[itr]
        else
        {
            itr++;
        }
    }

    // chop leading white space
    for (auto i = tokens.begin(); i != tokens.end(); i++)
    {
        chomp((*i));
#ifdef DEBUG_STRINGS
        std::cout << "\"" << (*i) <<  "\"" << std::endl;
#endif
    }
    /* -----------------------------------
     * -- build tree from string vector --
     * ----------------------------------- */

    std::vector<std::shared_ptr<Element>> elements{tokens.size()};
    std::shared_ptr<Element> temp_s, temp_f;
    for (int i = tokens.size() - 1; i > -1; i--)
    {
        // set temps to nullptr
        temp_s.reset();
        temp_f.reset();

#ifdef DEBUG_CREATION
        std::cout << "Parsing \"" << tokens[i] << "\"" << std::endl;
#endif
        if (tokens[i].substr(tokens[i].length() - 2) == "&&") // check if &&
        {
            tokens[i] = tokens[i].substr(0, tokens[i].length() - 2);
            chomp(tokens[i]);
#ifdef DEBUG_CREATION
            std::cout << "Creating AND  connector from \"" << tokens[i] << "\"" << std::endl;
#endif
            if (i + 1 < elements.size())
                temp_s = elements[i + 1];
            if (i + 2 < elements.size())
                temp_f = elements[i + 2];
            elements[i] = std::shared_ptr<Element>(new AndConnector(tokens[i], temp_s, temp_f, input, output));
        }
        else if (tokens[i].substr(tokens[i].length() - 2) == "||") // check if ||
        {
            tokens[i] = tokens[i].substr(0, tokens[i].length() - 2);
            chomp(tokens[i]);
#ifdef DEBUG_CREATION
            std::cout << "Creating OR   connector from \"" << tokens[i] << "\"" << std::endl;
#endif
            if (i + 2 < elements.size())
                temp_s = elements[i + 2];
            if (i + 1 < elements.size())
                temp_f = elements[i + 1];
            elements[i] = std::shared_ptr<Element>(new OrConnector(tokens[i], temp_s, temp_f, input, output));
        }
        else if (tokens[i].substr(tokens[i].length() - 1) == ";") // check if ;
        {
            tokens[i] = tokens[i].substr(0, tokens[i].length() - 2);
            chomp(tokens[i]);
#ifdef DEBUG_CREATION
            std::cout << "Creating SEMI connector from \"" << tokens[i] << "\"" << std::endl;
#endif
            if (i + 1 < elements.size())
                temp_s = elements[i + 1];
            elements[i] = std::shared_ptr<Element>(new SemiConnector(tokens[i], temp_s, input, output));
        }
        else if (tokens[i].substr(tokens[i].length() - 1) == ")")
        {
#ifdef DEBUG_CREATION
            std::cout << "Creating PAREN from \"" << tokens[i] << "\"" << std::endl;
#endif
            tokens[i] = tokens[i].substr(1, tokens[i].length() - 2);
            chomp(tokens[i]);
            elements[i] = std::shared_ptr<Element>(new Paren(tokens[i], input, output));
        }
        else if (tokens[i].find(" > ") != -1)
        {
#ifdef DEBUG_CREATION
            std::cout << "Creating OUTPUTOVERWRITE from \"" << tokens[i] << "\n" << std::endl;
#endif
            chomp(tokens[i]);
            elements[i] = std::shared_ptr<Element>(new OutputOverwrite(tokens[i], input));
        }
        else if (tokens[i].find(" >> ") != -1)
        {
#ifdef DEBUG_CREATION
            std::cout << "Creating OUTPUTAPPEND from \"" << tokens[i] << "\n" << std::endl;
#endif
            chomp(tokens[i]);
            elements[i] = std::shared_ptr<Element>(new OutputAppend(tokens[i], input));
        }
        else
        {
            std::vector<std::string> v;
            int is_pipe = 0;
            int redirect = 0;
            int s = 0;
            int itr = 0;
            while(itr < tokens[i].length())
            {
                if (tokens[i][itr] == '"')
                {
                    closed = 0;
                    while (!closed)
                    {
                        itr = line.find('\"', itr + 1); // move itr forward to next "
                        if (itr == std::string::npos)   // could not find last "
                        {
                            perror("Error: Reached end of command without finding terminal '\"'");
                            return nullptr;
                        }
                        if (line[itr -1] != '\\')   // check if terminal " not \"
                        {
                            closed = 1;
                        }
                    }
                    itr++;
                }
                else if (tokens[i][itr] == ' ')
                {
                    v.push_back(tokens[i].substr(s, itr - s));
                    s = itr + 1;
                }
                else if(tokens[i][itr] == '|')
                {
                    is_pipe = 1;
                }
                else if(tokens[i][itr] == '<')
                {
                    redirect = 1;
                }
                itr++;
            }
            v.push_back(tokens[i].substr(s));

            if (v[0] == "[" || v[0] == "test")
            {
#ifdef DEBUG_CREATION
                std::cout << "Creating TEST  from \"" << tokens[i] << "\"" << std::endl;
#endif
                v.erase(v.begin());
                if (v[v.size() - 1] == "]")
                    v.erase(v.end() - 1);

                elements[i] = std::shared_ptr<Element>(new RTest(v));
            }
            else if (is_pipe)
            {
#ifdef DEBUG_CREATION
                std::cout << "Creating PIPE from \"" << tokens[i] << "\"" << std::endl;
#endif
                elements[i] = std::shared_ptr<Element>(new Pipe(tokens[i], input, output));
            }
            else if (redirect)
            {
#ifdef DEBUG_CREATION
                std::cout << "Creating INPUTREDIRECT from \"" << tokens[i] << "\"" << std::endl;
#endif
                elements[i] = std::shared_ptr<Element>(new InputRedirect(v, output));
            }
            else
            {
                elements[i] = std::shared_ptr<Element>(new Operatable(v, input, output));
            }
        }

    }

    return elements[0];
}

#ifdef DEBUG_MAIN

int main()
{
    std::shared_ptr<Element> element;
    std::string line;

    std::cout << "Enter Command: ";
    getline(std::cin, line);

    element = parse(line, nullptr, nullptr);
    if (!element)
        exit(1);
    element->print();
    
    element->execute();

    element.reset();
}

#endif
