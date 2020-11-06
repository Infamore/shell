#ifndef __ELEMENT__
#define __ELEMENT__

class Element
{
public:
    virtual int execute() = 0;
    virtual void print() {};

    virtual ~Element() {};
};

#endif // __ELEMENT__
