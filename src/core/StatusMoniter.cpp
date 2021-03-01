/** *****************************************************************************
*    @File      :  StatusMoniter.cpp
*    @Brief     :  程序运行状态监视器的基本实现。
*
** ******************************************************************************/
#include "StatusMoniter.h"

using namespace std::chrono;


void  StatusMoniter::startRunTime()
{
    m_startTime = system_clock::now();
}

void  StatusMoniter::startRunTime()
{
    m_stopTime = system_clock::now();
    auto duration = duration_cast<microseconds>(m_stopTime - m_startTime);
    m_status.timeCost = double(duration.count())/1000.0;  // 转为 (ms)。
}

MapStrDbl StatusMoniter::getStatus() const
{
    MapStrDbl status;
    status["state"]       = m_status.state;
    status["timeCost"]    = m_status.timeCost;
    status["cpuCost"]     = m_status.cpuCost;
    status["cpuUsage"]    = m_status.cpuUsage;
    status["peakMemCost"] = m_status.peakMemCost;
    status["memUsage"]    = m_status.memUsage;
}


#ifdef _WIN64
#pragma comment(lib,"psapi.lib")


void  WinStatusMoniter::startMoniter()
{
    startRunTime();
    startCpuTime();
}

void  WinStatusMoniter::stopMoniter(int state)
{
    stopRunTime();   
    stopCpuTime();
    memCostMoniter();

    m_status.state = state;
}

void WinStatusMoniter::startCpuTime()
{
    GetSystemTimes(&m_startIdleTime, &m_startKernelTime, &m_startUserTime);
}

void WinStatusMoniter::stopCpuTime()
{
    /* 获取cpu使用状态。
    *  GetSystemTimes：获取CPU的空闲、内核、用户使用时间
    *  程序cpu占用时间：CPU内核时间 + 用户使用时间 - CPU空闲时间
    *  程序cpu使用率  ：程序cpu占用时间/cpu运行时间
    */
    GetSystemTimes(&m_stopIdleTime, &m_stopKernelTime, &m_stopUserTime);

    auto idleTime   = turnInt64(m_stopIdleTime) - turnInt64(m_startIdleTime);
    auto kernelTime = turnInt64(m_stopKernelTime) - turnInt64(m_stopKernelTime);
    auto userTime   = turnInt64(m_stopUserTime) - turnInt64(m_startUserTime);

    if(kernelTime + userTime == 0) 
    {
        m_status.cpuCost  = 0;
        m_status.cpuUsage = 0;
    }
    else
    {
        m_status.cpuCost  = kernelTime + userTime - idleTime;
        m_status.cpuUsage = 1.0*m_status.cpuCost/(kernelTime + userTime);
    }
}

void WinStatusMoniter::memCostMoniter()
{
    /* 获取程序峰值内存使用。*/
    HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS_EX pmc = {0};
	if (!GetProcessMemoryInfo(handle,(PROCESS_MEMORY_COUNTERS*)&pmc,sizeof(pmc)))
	{
		cout << "GetProcessMemoryInfo fail, lastErrorCode: " ;
        cout << GetLastError() << endl;
	}    
    m_status.peakMemCost = pmc.PeakWorkingSetSize/1024.0;

    /* 获取程序内存使用率。*/
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatusEx(&memStatus);
    m_status.memUsage = 0.01*memStatus.dwMemoryLoad;
}

__int64 WinStatusMoniter::turnInt64(const FILETIME &ftime)
{
    LARGE_INTEGER li;
    li.LowPart  = ftime.dwLowDateTime;
    li.HighPart = ftime.dwHighDateTime;
    return li.QuadPart;
}

#elif __linux__
    // Nothing to do under linux.
#elif __APPLE__
    // Nothing to do under apple. 
#elif __ANDROID__
    // Nothing to do under android.
#endif
