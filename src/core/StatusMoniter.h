/** *****************************************************************************
*
*    @File      :  AppStatusCounter.h
*    @License   :  Apache License 2.0
*
*    @Version   :  1.0
*    @Date      :  2021/03/01
*    @Author    :  Qin ZhaoYu 
*    @Email     :  zhaoyu.qin@foxmail.com
*
*    @Desc      :  程序运行状态监视器，用于统计程序执行中cpu、内存使用情况以及程序耗时。  
*
*    Change History:
*  ------------------------------------------------------------------------------
*  --version1.0, 2021/03/01, Qin ZhaoYu
*  Nothing.
*  ------------------------------------------------------------------------------
*
** ******************************************************************************/
#ifndef APP_STATUS_COUNTER_H
#define APP_STATUS_COUNTER_H

#include <chrono>
#include "common/CommHeader.h"


class AppStatusCounter
{
public:    
    struct AppStatus
    {
        double state       =-1;  ///< 程序返回状态（=0, 成功；non-0，失败）
        double timeCost    =-1;  ///< 程序运行耗时（ms）
      
        double cpuCost     =-1;  ///< cpu占用时间（ms）
        double cpuUsage    =-1;  ///< cpu利用率（0~1）
        double peakMemCost =-1;  ///< 内存峰值占用（KB）
        double memUsage    =-1;  ///< 内存利用率（0~1）
    };

    using TimePoint = std::chrono::_V2::system_clock::time_point;

public:
    AppStatusCounter();
    virtual ~AppStatusCounter();

    /**
    * @brief 开始程序状态统计。
    */
    virtual void      startCounter() =0;
    /**
    * @brief 结束程序状态统计。
    */
    virtual void      stopCounter(int state) =0;

    /**
    * @brief 获取程序运行状态。
    * 状态包括{"state", "timeCost", "cpuCost", "cpuUsage", * "maxMemCost", "memUsage"}。
    */
    virtual MapStrDbl getStatus() const;

protected:
    void startAppRunTime();
    void stopAppRunTime();

protected:
    TimePoint   m_startTime;  ///< 程序运行起始时间
    TimePoint   m_stopTime;   ///< 程序运行终止时间
    AppStatus   m_status;     ///< 程序运行状态
};


#ifdef _WIN64
#include <windows.h>
#include <psapi.h>


class WinAppStatusCounter:public AppStatusCounter
{
public:
    void  startCounter()          override;
    void  stopCounter(int state)  override;

private:
    void startCpuTime();
    void stopCpuTime();
    void memCostCounter();

    __int64 turnInt64(const FILETIME &ftime);

private:
    FILETIME  m_startIdleTime;    // 开始时cpu闲置时间
    FILETIME  m_startKernelTime;  // 开始时cpu内核时间
    FILETIME  m_startUserTime;    // 开始时cpu用户使用时间
    FILETIME  m_stopIdleTime;     // 结束时cpu闲置时间
    FILETIME  m_stopKernelTime;   // 结束时cpu内核时间
    FILETIME  m_stopUserTime;     // 结束时cpu用户使用时间
};




#elif __linux__
    // Nothing to do under linux.
#elif __APPLE__
    // Nothing to do under apple. 
#elif __ANDROID__
    // Nothing to do under android.
#else
    printf("\n\n---*--- Unsupported System Platform. ---*---\n\n")
#endif



#endif