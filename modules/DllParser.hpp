/** *****************************************************************************
*   @copyright :  Copyright (C) 2022 Qin ZhaoYu. All rights reserved.
*
*   @author    :  Qin ZhaoYu.
*   @see       :  https://github.com/QINZHAOYU
*   @brief     :  To implement dll parser to simplify the dll function calling.
*
*   Change History:
*   -----------------------------------------------------------------------------
*   v1.0, 2022/01/04, Qin ZhaoYu, zhaoyu.qin@foxmail.com
*   Init model.
*
** ******************************************************************************/
#pragma once
#include "common/CommHeader.hpp"
#include <map>
#include <functional>


namespace ccb
{

#ifdef WINDOWS
/// \brief Dll parser and wrapper.
class DllParser
{
public:
    DllParser();
    ~DllParser();

    bool Load(const string &dllPath);
    bool UnLoad();

    template<typename T>
    std::function<T> GetFunction(const string &funcName)
    {
        if (_hMod == nullptr)
        {
            cout << "Dll not loaded.\n";
            return nullptr;
        }

    auto it = _map.find(funcName);
    if (it == _map.end())
    {
        auto addr = GetProcAddress(_hMod, funcName.c_str());
        if (!addr)
        {
            cout << "Can find this function " << funcName << endl;
            return nullptr;
        }

        _map.insert(std::make_pair(funcName, addr));
        it = _map.find(funcName);
    }

    return std::function<T> ((T*)(it->second));
}

    template<typename T, typename... Args>
    typename std::result_of<std::function<T>(Args...)>::type 
    ExcecuteFunc(const string &funcName, Args&& ...args)
{
    auto f = GetFunction<T>(funcName);
    if (f == nullptr)
    {
        string msg = "Invalid function name " + funcName;
        throw std::logic_error(msg.c_str());
    }

    return f(std::forward<Args>(args)...);
}

private:
    HMODULE _hMod;
    std::map<string, FARPROC> _map;
};

#elif defined(LINUX)

/// \brief Dll parser and wrapper on linux.
class DllParser
{
public:
    DllParser();
    ~DllParser();

    bool Load(const string &dllPath);
    bool UnLoad();

    template<typename T>
    std::function<T> GetFunction(const string &funcName)
    {
        if (_hMod == nullptr)
        {
            cout << "Dll not loaded.\n";
            return nullptr;
        }

    auto it = _map.find(funcName);
    if (it == _map.end())
    {
        auto addr = dlsym(_hMod, funcName.c_str());
        if (!addr)
        {
            cout << "Can find this function " << funcName << endl;
            return nullptr;
        }

        _map.insert(std::make_pair(funcName, addr));
        it = _map.find(funcName);
    }

    return std::function<T> ((T*)(it->second));
}

    template<typename T, typename... Args>
    typename std::result_of<std::function<T>(Args...)>::type 
    ExcecuteFunc(const string &funcName, Args&& ...args)
{
    auto f = GetFunction<T>(funcName);
    if (f == nullptr)
    {
        string msg = "Invalid function name " + funcName;
        throw std::logic_error(msg.c_str());
    }

    return f(std::forward<Args>(args)...);
}

private:
    using Parser_t = void*;

    Parser_t _hMod;
    std::map<string, Parser_t> _map;
};

#endif

}

