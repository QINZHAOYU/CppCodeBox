/** *******************************************************************************
*    @File      :  TaskBatcher.cpp
*    @Brief     :  任务批处理器基本实现。
*
** ********************************************************************************/
#include <chrono>
#include "TaskBatcher.h"
#include "AppRunFlow.h"
#include "ModelRegister.h"

using namespace chrono;


TaskBatcher::TaskBatcher(int argc, const char* argv[])
{
    m_ver = 1.0；
    m_cmdFmt = "<ExeFile> // <model> arg1 arg2 ... // <model> arg1 arg2 ...\n";
    
    parseCmd(argc, argv);
}

bool TaskBatcher::init()
{
    if(!m_cmdGroup.empty()) return false;
    else return true;
}

bool TaskBatcher::run()
{
    for(auto &args: m_cmdGroup)
    {
        auto start = system_clock::now();

        string name = args[0];
        AppRunFlow* app = ModelRegister::getInstance()->create(name);
        int state = app->run(args.size(), args);

        auto end = system_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        TaskStats stats;
        stats.isFailed = state;
        stats.timeCost = double(duration.count())/1000;
        m_statses.push_back(stats);
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
}

MapStrDbl TaskBatcher::getTaskStats(int i)
{
    MapStrDbl stats;
    if(i < 0 || i >= m_statses.size()) MapStrDbl();

    stats["isFaild"] = m_statses[i].isFailed;
    stats["timeCost"] = m_statses[i].timeCost;
    stats["MaxMemCost"] = m_statses[i].MaxMemCost;
    stats["AveMemCost"] = m_statses[i].AveMemCost;
    return stats;
}

void TaskBatcher::helper(const string &item)
{
    switch(item)
    {
        case "-v":
        case "-ver":
        case "-version":
            cout<<"Task Batcher Version: "<<m_ver<<endl;
            break;
        case "-f":
        case "-fmt":
        case "-format":
            cout<<"Task Batcher Format: "<<m_cmdFmt<<endl;
            break;
        default:
            cout<<"Please input -ver or -fmt to get help.\n";
    }
}
