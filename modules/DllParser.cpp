/** *******************************************************************************
*    @File      :  DllParser.cpp
*    @Brief     :  None.
*
** ********************************************************************************/
#include "DllParser.hpp"

using namespace ccb;


// class DllParser -------------------------------------------------------------
#ifdef WINDOWS

DllParser::DllParser(): _hMod(nullptr)
{}

DllParser::~DllParser()
{
    UnLoad();
}

bool DllParser::Load(const string &dllPath)
{
    _hMod = LoadLibraryA(dllPath.data());
    if (_hMod == nullptr)
    {
        cout << "LoadLibrary failed.\n";
        return false;
    }

    return true;
}

bool DllParser::UnLoad()
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


// class LinuxDllParser -----------------------------------------------------------
#else defined(LINUX)

DllParser::DllParser(): _hMod(nullptr)
{}

DllParser::~DllParser()
{
    UnLoad();
}

bool DllParser::Load(const string &dllPath)
{
    _hMod = dlopen(dllPath.c_str(), RTLD_LAZY);
    if (_hMod == nullptr)
    {
        cout << "LoadLibrary failed.\n";
        return false;
    }

    return true;
}

bool DllParser::UnLoad()
{
    if (_hMod == nullptr)
    {
        return true;
    }

    int status = dlclose(_hMod);
    if (status != 0)
    {
        return false;
    }

    _hMod = nullptr;
    _map.clear();
    return true;
}


#endif





