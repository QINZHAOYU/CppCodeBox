/** *******************************************************************************
*
*    @File      :  main.cpp
*    @License   :  LGPL-3.0
*
*    @Version   :  1.0
*    @Date      :  2021/02/05
*    @Author    :  Qin ZhaoYu 
*    @Email     :  zhaoyu.qin@foxmail.com
*
*    @Desc      :  程序入口，程序采用模块化设计和编译，主要用于新技术和功能原型测试。  
*
*    Change History:
*  --------------------------------------------------------------------------------
*  --version1.0, 2021/02/05, Qin ZhaoYu
*  初始构建。
*  --------------------------------------------------------------------------------
*
** ********************************************************************************/
#include "src/core/AppRunFlow.h"



int main(int argc, const char *argv[])
{
    int state = 0;

    try
    {
        AppRunFlow task;
        state = task.run(argc, argv);
        for(auto &it: task.appInfo())
            printf("%f: %f\n", it->first, it->second);

    }catch(...)
    {
        printf("---*--- Task Run Failed.\n");
    }

    return state;
}
