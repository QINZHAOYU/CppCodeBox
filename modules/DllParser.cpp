/** *******************************************************************************
*    @File      :  DllParser.cpp
*    @Brief     :  None.
*
** ********************************************************************************/
#include "DllParser.hpp"

using namespace ccb;


WinDllParser::WinDllParser(): _hMod(nullptr)
{}

WinDllParser::~WinDllParser()
{
    UnLoad();
}

bool WinDllParser::Load(const string &dllPath)
{
    _hMod = LoadLibraryA(dllPath.data());
    if (_hMod == nullptr)
    {
        cout << "LoadLibrary failed.\n";
        return false;
    }

    return true;
}

bool WinDllParser::UnLoad()
{
    if (_hMod == nullptr)
    {
        return true;
    }

    auto status = FreeLibrary(_hMod);
    if (!status)
    {
        return false;
    }

    _hMod = nullptr;
    _map.clear();
    return true;
}

// template<typename T>
// std::function<T> WinDllParser::GetFunction(const string &funcName)
// {
//     auto it = _map.find(funcName);
//     if (it == _map.end())
//     {
//         auto addr = GetProcAddress(_hMod, funcName.c_str());
//         if (!addr)
//         {
//             cout << "Can find this function " << funcName << endl;
//             return nullptr;
//         }

//         _map.insert(std::make_pair(funcName, addr));
//         it = _map.find(funcName);
//     }

//     return std::function<T> ((T*)(it->second));
// }

// template<typename T, typename... Args>
// typename std::result_of<std::function<T>(Args...)>::type 
// WinDllParser::ExcecuteFunc(const string &funcName, Args&& ...args)
// {
//     auto f = GetFunction<T>(funcName);
//     if (f == nullptr)
//     {
//         string msg = "Invalid function name " + funcName;
//         throw std::exception(msg);
//     }

//     return f(std::forward<Args>(args)...);
// }
