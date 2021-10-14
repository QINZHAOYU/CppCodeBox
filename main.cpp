/** *****************************************************************************
*   @copyright :  Copyright (C) 2021 Qin ZhaoYu. All rights reserved.
*
*   @author    :  Qin ZhaoYu.
*   @brief     :  To collect useful toolkits and modules; to test new technique.
*
*   Change History:
*   -----------------------------------------------------------------------------
*   --version-1.0, 2021/10/09, Qin ZhaoYu,
*   To init model.
*
** ******************************************************************************/

#include "common/CommHeader.hpp"


int main(int argc, char* argv[])
{    
    try
    {

        cout << "------------- started." << endl;

        typedef void(*Func)(void);
#if defined(WINDOWS)
    HINSTANCE dll = LoadLibraryA("../bin/MyCppKits.dll");
    if (dll)
    {
        Func myFunc = (Func) GetProcAddress(dll, "printHello");
        if (myFunc) 
        {
            myFunc();
        }
        else
        {
            cout << "function not found." << endl;
        }
        FreeLibrary(dll);
    }
    else
    {
        cout << "dll not loaded." << endl;
    }
#elif defined(LINUX)
    dlerror();

    void *dll = dlopen("tools/test.so",RTLD_LAZY);
    if (!dll)
    {
        cout << "so not loaded." << endl;
    }

    Func myFunc = (Func) dlsym(dll,"printHello");

    const char *msg = dlerror();
    if(msg)
    {
        cout << "function not found." << endl << msg << endl;
        dlclose(dll);
    }
    else
    {
        myFunc();
    }	
 
    dlclose(dll);
#endif
    
    cout << "------------- end." << endl;

    }
    catch(...)
    {
        cout << "err" << endl;
    }

    return 0;
}

