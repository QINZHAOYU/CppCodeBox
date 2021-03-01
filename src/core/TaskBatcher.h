/** *******************************************************************************
*
*    @File      :  TaskBatcher.h
*    @License   :  LGPL-3.0
*
*    @Version   :  1.0
*    @Date      :  2021/02/05
*    @Author    :  Qin ZhaoYu 
*    @Email     :  zhaoyu.qin@foxmail.com
*
*    @Desc      :  任务批处理器，实现命令行多任务批处理，同时提供各任务统计信息。 
*
*    Change History:
*  --------------------------------------------------------------------------------
*  --version1.0, 2021/02/05, Qin ZhaoYu
*  初步构建。
*  --------------------------------------------------------------------------------
*
** ********************************************************************************/
#ifndef TASK_BATCHER_H
#define TASK_BATCHER_H

#include "common/CommHeader.h"
#include "AppRunFlow.h"


class TaskBatcher
{
public:
    TaskBatcher(int argc, const char* argv[]);

    bool init();
    bool run();

    MapStrDbl getTaskStatus(int i);  

private:
    void parseCmd(int argc, const char* argv[]);
    void help();

private:
    using VecVecChar = vector<vector<const char*>>;
    using VecChar    = vector<const char*>;

private:
    int                     m_verion;    // 任务批处理器版本
    string                  m_cmdFmt;    // 命令行参数输入格式
    VecVecChar              m_cmdGroup;  // 命令行参数分组集合m_ver
    vector<MapStrDbl>       m_statuses;  // 任务统计信息
};


#endif