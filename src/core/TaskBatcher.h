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

    MapStrDbl getTaskStats(int i);  

private:
    void parseCmd(int argc, const char* argv[]);
    void helper();

private:
    using VecVecChar = vector<vector<const char*>>;
    using VecChar    = vector<const char*>;

    struct TaskStats
    {
        double   isFailed;     // 任务是否执行成功（=0, 成功；non-0，失败）
        double   timeCost;     // 任务耗时（ms）
        double   MaxMemCost;   // 任务最大内存占用（kB）
        double   AveMemCost;   // 任务平均内存占用（KB）
    };

private:
    int                     m_ver;       ///< 任务批处理器版本
    string                  m_cmdFmt;    ///< 命令行参数输入格式
    VecVecChar              m_cmdGroup;  ///< 命令行参数分组集合m_ver
    vector<TaskStats>       m_statses;   ///< 任务统计信息
};


#endif