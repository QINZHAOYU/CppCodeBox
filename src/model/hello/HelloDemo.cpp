#include "HelloDemo.h"

namespace hello
{
    int HelloDemo::run(int argc, const char* argv[])
    {
        prologue();

        while(!terminated())
        {
            step();
            m_step ++;
        }

        return 0;
    }

    map<string, string> HelloDemo::appInfo()
    {
        map<string, string> info;
    
        info["name"] = "HelloDemo";
        info["desc"] = "used for cmake practice";
        info.insert(std::pair<string, string>("version", "0.1"));
        info.insert(std::pair<string, string>("author", "Qin ZhaoYu"));
    
        return info;
    }

    void HelloDemo::prologue()
    {
        printf("=================================================\n");
        printf("--- * --- HelloDemo, ver0.1, Qin ZhaoYu --- * ---\n");
        printf("=================================================\n\n");
    }

    bool HelloDemo::terminated()
    {
        if (m_isStopping || m_step > 1)
            return true;
        else
            return false;
    }

    void HelloDemo::step()
    {
        scheme();
        m_isStopping = true;
    }

    void HelloDemo::scheme()
    {
        printf("This a demo for testing new framwork.\n")
    }
}
