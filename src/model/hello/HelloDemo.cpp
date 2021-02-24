/** *****************************************************************************
*    @File      :  HelloDemo.cpp
*    @Brief     :  None.
*
** ******************************************************************************/
#include "core/ModelRegister.h"
#include "HelloDemo.h"


#define HELLO "hello"
MODEL_REGISTER(HELLO, hello::HelloModel)


namespace hello
{
    int HelloModel::run(int argc, const char* argv[])
    {
        prologue();

        while(!terminated())
        {
            step();
        }

        return 0;
    }

    MapStrStr HelloModel::appInfo() const
    {    
        return m_info;
    }

    void HelloModel::prologue()
    {
        printf("=================================================\n");
        printf("--- * --- HelloModel, ver0.1 --- * ---\n");
        printf("=================================================\n");

        m_info["name"] = "HelloModel";
        m_info["desc"] = "used for cmake practice";
        m_info.insert(std::pair<string, string>("ver", "0.1"));
        m_info.insert(std::pair<string, string>("author", "Qin ZhaoYu"));        
    }

    void HelloModel::parseCmd(int argc, const char *argv[])
    {
        if(argc < 2)
            m_cmdStr = "Empty cmd args.";
        
        for(int i=1; i < argc; ++i)
            m_cmdStr += string(argv[i]);      
    }

    bool HelloModel::terminated()
    {
        if (m_isStop || m_step > 1)
            return true;
        else
            return false;
    }

    void HelloModel::step()
    {
        scheme();
        m_step++;
    }

    void HelloModel::scheme()
    {        
        printf("Command Line: %s\n", m_cmdStr.c_str());
        m_isStop = true;
    }
}
