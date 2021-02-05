/** *******************************************************************************
*
*    @File      :  AppRunFlow.h
*    @License   :  LGPL-3.0
*
*    @Version   :  1.0
*    @Date      :  2021/02/05
*    @Author    :  Qin ZhaoYu 
*
*    @Desc      :  抽象程序运行流程，规范后续模块接口开发，实现统一实现逻辑和交互接口。
*
*    Change History:
*  --------------------------------------------------------------------------------
*  --version1.0, 2021/02/05, Qin ZhaoYu
*  初步构建.
*  --------------------------------------------------------------------------------
*
** ********************************************************************************/
#ifndef APP_RUN_FLOW_H
#define APP_RUN_FLOW_H


#include "common/CommHeader.h"


class AppRunFlow
{
public:
    /**
    * @brief   程序入口，运行程序。 
    * 
    * @return  如果程序运行完成，返回 0；否则，返回 non-0 状态。
    */
    virtual int  run(int argc, const char *argv[]);

    /**
    * @brief   获取程序计算总步数。
    */
    virtual size_t getTotalSteps() {return m_step;}

    /**
    * @brief   获取程序基本信息,包括关键字{"name", "desc", "version", "author"}。 
    */
    virtual map<string, string>  appInfo();

protected:
    ////////////////////////////////////////////////////////////////
    // 参数解析与运行监视
    //
    // 打印前言，解析命令行参数，监视运行状态（时间等），启动程序。
    //
    virtual void prologue();
    virtual void parseCmd(int argc, const char *argv[]);
    virtual bool terminated();

    ////////////////////////////////////////////////////////////////
    // 初始化操作
    //
    // 完成程序各项初始化操作，包括前初始化、正式初始化、后置初始化。
    //
    virtual void preInit();
    virtual void init();
    virtual void postInit();

    ////////////////////////////////////////////////////////////////
    // 程序运行主循环
    //
    // 程序计算运行主循环，包括循环前后处理、每一步进操作，程序清理。
    //
    virtual void preLoop();
    virtual void inLoopPreStep();
    virtual void step();
    virtual void inLoopPostStep();
    virtual void postLoop();
    virtual void clear();

    ////////////////////////////////////////////////////////////////
    // 算法
    //
    // 模块核心算法实现，包括算法前后处理、算法核心步骤。
    //
    virtual void inStepPreScheme();
    virtual void scheme();
    virtual void inStepPostScheme();

protected:
    size_t       m_step;           ///< 运行步数。
    bool         m_isStopping;     ///< 状态标识符，标识程序是否终止。
}






#endif