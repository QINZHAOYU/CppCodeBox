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
#include "tools/MathFunctions/MathFunctions.hpp"
#include "modules/hello.hpp"
#include "modules/GraphSearchingAlgo.hpp"

int main(int argc, char *argv[])
{
	try
	{

		cout << "----------------------- started." << endl;

		printHello();
		cout << "my sqrt(): " << mysqrt(4) << endl;

		typedef void(*Func)(void);

#if defined(WINDOWS)
		string dllPath = "./bin/MyCppKits.dll";
		if (argc > 1)
		{
			dllPath = argv[1];
		}
		HINSTANCE dll = LoadLibraryA(dllPath.c_str());
		if (dll)
		{
			Func myFunc = (Func) GetProcAddress(dll, "printHello");
			if (myFunc)
			{
				myFunc();
			}
			else
			{
				cout << "dll function not found." << endl;
			}
			FreeLibrary(dll);
		}
		else
		{
			cout << "dll not loaded." << endl;
		}
#elif defined(LINUX)
		string dllPath = "./bin/MyCppKits.so";
		if (argc > 1)
		{
			dllPath = argv[1];
		}

		void *dll = dlopen(dllPath.c_str(), RTLD_LAZY);
		if (!dll)
		{
			cout << "dll not loaded." << endl;
		}

		Func myFunc = (Func) dlsym(dll, "printHello");

		const char *msg = dlerror();
		if (msg)
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

		cout << "----------------------- end." << endl;

	}
	catch (...)
	{
		cout << "error happened." << endl;
	}

	return 0;
}

