/** *******************************************************************************
*    @File      :  TaskBatcher.cpp
*    @Brief     :  任务批处理器基本实现。
*
** ********************************************************************************/
#include <string.h>
#include "ModelRegister.h"
#include "AppRunFlow.h"
#include "TaskBatcher.h"
#include "StatusMoniter.h"


TaskBatcher::TaskBatcher(int argc, const char* argv[])
{
    m_verion = 1.0;
    m_cmdFmt = "<ExeFile> arg1 arg2 ...// <model> arg1 arg2 ... // <model> arg1 arg2 ...\n";
    
    parseCmd(argc, argv);
}

bool TaskBatcher::init()
{
    if(m_cmdGroup.empty()) return false;
    else return true;
}

bool TaskBatcher::run()
{
    for(auto &args: m_cmdGroup)
    {
        StatusMoniter* status = getStatusMoniter();

        AppRunFlow* app = ModelRegister::getInstance()->create(args[0]);
        int state = app->run(args.size(), &args[0]);

        status->stopMoniter(state);
        m_statuses.push_back(status->getStatus());
        delStatusMoniter(status);
    }
}

void TaskBatcher::parseCmd(int argc, const char* argv[])
{
    VecChar args;
    for(int i=1; i< argc; ++i)
    {
        // 以“//”作为不同任务参数组的分隔符
        if(strcmp(argv[i], "//") == 0){
            if(args.empty()) continue;
            m_cmdGroup.push_back(args);
            args.clear();
        }
        else{
            args.push_back(argv[i]);
        }            
    }

    if(m_cmdGroup.size() < 2) help();
}

MapStrDbl TaskBatcher::getTaskStatus(int i)
{
    if(i < 0 || i >= m_statuses.size()) return MapStrDbl();

    return m_statuses[i];
}

void TaskBatcher::help()
{
    std::cout<<"---*--- Task Batcher ---*---\n";

    std::cout<<"::=> Version: "<<m_verion<<std::endl;
    std::cout<<"::=> Format: "<<m_cmdFmt<<std::endl;

    std::cout<<"----------------------------\n";
}
