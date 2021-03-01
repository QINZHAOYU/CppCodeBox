/** *****************************************************************************
*
*    @File      :  CppCodeBox.cpp
*    @License   :  Apache License 2.0
*
*    @Version   :  1.0
*    @Date      :  2021/02/05
*    @Author    :  Qin ZhaoYu 
*    @Email     :  zhaoyu.qin@foxmail.com
*
*    @Desc      :  程序入口，主要用于测试新技术和功能原型。  
*
*    Change History:
*  ------------------------------------------------------------------------------
*  --version1.0, 2021/02/05, Qin ZhaoYu
*  初始构建。
*  ------------------------------------------------------------------------------
*
** ******************************************************************************/

#include "core/TaskBatcher.h"


int main(int argc, const char *argv[])
{
    int state = 0;

    try
    {
        TaskBatcher tasks(argc, argv);

        int state = (tasks.init())?tasks.run():false;

        if(state == 0) 
            cout<<"run tasks sucessed";
        else
            cout<<"run tasks failed.";
        cout<<"---*--- Task Run Done.\n";

    }catch(...)
    {
       cout<<"---*--- Task Run Failed.\n";
    }

    return state;
}
