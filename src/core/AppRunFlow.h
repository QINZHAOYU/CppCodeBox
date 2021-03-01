/** *******************************************************************************
*
*    @File      :  AppRunFlow.h
*    @License   :  LGPL-3.0
*
*    @Version   :  1.0
*    @Date      :  2021/02/05
*    @Author    :  Qin ZhaoYu 
*    @Email     :  zhaoyu.qin@foxmail.com
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

#ifdef _DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif


#include "common/CommHeader.h"


class AppRunFlow
{
public:
    AppRunFlow();
    virtual ~AppRunFlow();

    /**
    * @brief   命令行调用程序，封装程序运行全过程。 
    * 
    * @return  如果程序运行完成，返回 0；否则，返回 non-0 状态。
    */
    virtual int        run(int argc, const char *argv[]);

    /**
    * @brief   分步运行时，加载配置并且初始化。
    *
    * @param[in] tokens    程序初始化参数集。
    * @return  如果加载成功则返回True;否则，返回False。
    */
    virtual bool       load(const VecStr &tokens) {return false;}

    /**
    * @brief   分步运行时，执行步进运算。
    *
    * @param[in] i         当前步。
    * @return  如果当前步运行成功则返回True;否则，返回False。
    */    
    virtual bool       runByStep(int i)           {return false;}

    /**
    * @brief   分步运行时，设置程序状态变量或常量。
    */
    virtual bool       setState(const string &item,  const VecDbl &states) {return false;}
    virtual bool       setStepState(int i, const string &item, double val) {return false;}

    /**
    * @brief   获取程序状态变量或常量。
    */
    virtual VecDbl     getState(const string &item)            const {return VecDbl();}
    virtual double     getStepState(int i, const string &item) const {return -9999;}

    /**
    * @brief   获取程序计算总步数。
    */
    virtual size_t     getTotalSteps() const {return m_steps;}

    /**
    * @brief   获取程序基本信息,包括关键字{"name", "desc", "ver", "author"}。 
    */
    virtual MapStrStr  appInfo()       const {return m_info;}
    virtual string     appInfo(const string &item) const;

protected:
    ////////////////////////////////////////////////////////////////
    // 参数解析与运行监视
    //
    // 打印前言，解析命令行参数，监视运行状态，提供帮助等，启动程序。
    //
    virtual void prologue();
    virtual void parseCmd(int argc, const char *argv[]);
    virtual bool terminated();
    virtual void help(string item);

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
    bool         m_isInit;    ///< 状态标识符，标识程序是否初始化加载。
    bool         m_isStop;    ///< 状态标识符，标识程序是否终止。

    size_t       m_steps;     ///< 运行总步数。   
    MapStrVec    m_states;    ///< 状态数据集。
    MapStrStr    m_info;      ///< 程序基本信息。
};


// 提供动态库导出接口
extern "C" DLL_API  AppRunFlow* createApp();
extern "C" DLL_API  void        deleteApp(AppRunFlow* app) {if(!app) delete app;}


#endif