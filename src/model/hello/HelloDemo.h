/** *****************************************************************************
*
*    @File      :  HelloDemo.h
*    @License   :  Apache License 2.0
*
*    @Version   :  1.0
*    @Date      :  2021/02/24
*    @Author    :  Qin ZhaoYu 
*    @Email     :  zhaoyu.qin@foxmail.com
*
*    @Desc      :  None  
*
*    Change History:
*  ------------------------------------------------------------------------------
*  --version1.0, 2021/02/24, Qin ZhaoYu
*  Nothing.
*  ------------------------------------------------------------------------------
*
** ******************************************************************************/
#ifndef HELLO_DEMO_H
#define HELLO_DEMO_H

#include "core/AppRunFlow.h"


namespace hello
{
    class HelloModel:public AppRunFlow
    {
    public:
        int        run(int argc, const char *argv[])       override;
        MapStrStr  appInfo() const                         override;
 
    private: 
        void       prologue()                              override;
        void       parseCmd(int argc, const char *argv[])  override;
        bool       terminated()                            override;
        void       step()                                  override;
        void       scheme()                                override;

    private:
        string     m_cmdStr;
        size_t     m_step;
    };
}


#endif