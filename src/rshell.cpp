#include <iostream>
#include <string>

#include "BaseConnector.hpp"
#include "Operatable.hpp"
#include "andconnector.hpp"
#include "orconnector.hpp"
#include "semiconnector.hpp"
#include "element.hpp"

int main()
{
    std::string command;
    std::shared_ptr<Element> run;
    while (1)
    {
        std::cout << "$ ";
        std::getline(std::cin, command);

        // quit if "exit"
        if (command == "exit")
        {
            std::cout << "-- Shell Exiting --" << std::endl;
            exit(0);
        }
        // clear screen if clear
        else if (command == "clear")
        {
            std::cout << "\033[0;0H\033[2J";
        }
        else
        {
            // command processing here
            int closed;
            for (int i = 0; i < command.length(); i++)
            {
                if (command[i] == '"') // skip strings in commands
                {
                    closed = 0;
                    while (!closed)
                    {
                        i = command.find('\"', i + 1);      // move itr forward to next "
                        if (command[i -1] != '\\')   // check if terminal " not \"
                        {
                            closed = 1;
                        }
                    }
                    i++;
                }
                else if (command[i] == '#')
                {
                    command = command.substr(0, i);
                    command = command.substr(0, command.find_last_not_of(" ") + 1);
                    break;
                }
            }
            if (command.length() != 0)
            {
                run = parse(command, nullptr, nullptr);
                //run->print();
                //std::cout << "-------------------------------" << std::endl;
                run->execute();
            }
        }
    }
}
