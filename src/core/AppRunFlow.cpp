/** *******************************************************************************
*    @File      :  AppRunFlow.cpp
*    @Brief     :  程序抽象运行流程的基本实现。
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
        inLoopPostStep();
    }
    postLoop();

    clear();
    return 0;
}


void AppRunFlow::step()
{
    inStepPreScheme();
    scheme();
    inLoopPostStep();
}


string  AppRunFlow::appInfo(const string &item) const
{
    auto info = m_info.find(item);
    if(info != m_info.end()){
        return info->second;
    }

    return "";
}





