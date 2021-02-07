#ifndef HELLO_DEMO_H
#define HELLO_DEMO_H

#include "core/AppRunFlow.h"


namespace hello
{
    class HelloDemo:public AppRunFlow
    {
    public:
        int run(int argc, const char *argv[]) override;
        map<string, string>  appInfo() const  override;

    private:
        void prologue()                       override;
        bool terminated()                     override;
        void step()                           override;
        void scheme()                         override;
    }
}


#endif