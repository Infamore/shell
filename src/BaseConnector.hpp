#ifndef __BASE_CONNECTOR__
#define __BASE_CONNECTOR__

#include "element.hpp"
#include <iostream>
#include <memory>

//#define DEBUG_OPERATE
//#define DEBUG_DESTRUCT

class BaseConnector : public Element
{
protected:
    std::shared_ptr<Element> left = nullptr;
    std::shared_ptr<Element> success = nullptr;
    std::shared_ptr<Element> failure = nullptr;

    int input[2], output[2];

public:

    virtual ~BaseConnector()
    {
#ifdef DEBUG_DESTRUCT
        std::cout << "Deconstructing: " << this << std::endl;
#endif

        left.reset();

#ifdef DEBUG_DESTRUCT
        std::cout << success << " remaining references: " << success.use_count() - 1 << std::endl;
        std::cout << failure << " remaining references: " << failure.use_count() - 1 << std::endl;
#endif
        success.reset();
        failure.reset();
        
    }

    virtual int execute()
    {
        int status = left->execute(); // there should always be a left side
        if (status == 0) // 0 is proper execution
        {
#ifdef DEBUG_OPERATE
            std::cout << "Success" << std::endl;
#endif      
            if (success) // if success pointer
            {
                status = success->execute(); // execute success
            }
        }
        else // left failed
        {
#ifdef DEBUG_OPERATE
            std::cout << "Failure" << std::endl;
#endif
            if (failure) // if failure pointer
            {
                status = failure->execute(); // execute failure
            }
        }
        return status; // returns success of last operated element
    }

    virtual void print() {}
};

#endif // __BASE_CONNECTOR__
