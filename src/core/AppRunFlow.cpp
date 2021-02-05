/** *******************************************************************************
*    @File      :  AppRunFlow.cpp
*    @Brief     :  抽象程序运行流程的基本实现。
*
** ********************************************************************************/
#include "AppRunFlow.h"


int AppRunFlow::run(int argc, const char* argv[])
{
    prologue();
    parseCmd(argc, argv);

    preLoop();
    while(!(terminated()))
    {
        inLoopPreStep();
        step();
        ++m_step;
        inLoopPostStep();
    }
    postLoop()

    clear();
    return 0;
}


void AppRunFlow::step()
{
    inStepPreScheme();
    scheme();
    inLoopPostStep();
}


map<string, string>  AppRunFlow::appInfo()
{
    map<string, string> info;
    
    info["name"] = "";
    info["desc"] = "";
    info.insert(std::pair<string, string>("version", ""));
    info.insert(std::pair<string, string>("author", ""));

    return info;
}





