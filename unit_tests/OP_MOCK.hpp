#ifndef __OP_MOCK__
#define __OP_MOCK__

#include "../src/element.hpp"

#include <iostream>
#include <fstream>
#define TRUE 0
#define FALSE 1

class OP_MOCK : public Element
{
    int val;

    public:
        OP_MOCK(int v): val(v) {}

        virtual int execute()
        {
            std::ofstream out;
            out.open("testfile");
            out << val << std::endl;
            out.close();
            return val;
        }

        virtual void print()
        {
            if (val)
            {
                std::cout << "FALSE" << std::endl;
            }
            else
            {
                std::cout << "TRUE" << std::endl;
            }
        }

};

#endif
