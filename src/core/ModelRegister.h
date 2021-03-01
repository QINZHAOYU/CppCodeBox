/** *****************************************************************************
*
*    @File      :  ModelRegister.h
*    @License   :  Apache License 2.0
*
*    @Version   :  1.0
*    @Date      :  2021/02/24
*    @Author    :  Qin ZhaoYu 
*    @Email     :  zhaoyu.qin@foxmail.com
*
*    @Brief     :  模块注册器，实现模块、算法注册。  
*
*    Change History:
*  ------------------------------------------------------------------------------
*  --version1.0, 2021/02/24, Qin ZhaoYu
*  ------------------------------------------------------------------------------
*
** ******************************************************************************/
#ifndef MODEL_REGISTER_H
#define MODEL_REGISTER_H

#include "common/CommHeader.h"

class AppRunFlow;


class ModelFactory
{
public:
    virtual ~ModelFactory()     {}
    virtual AppRunFlow* create()=0;
};


class ModelRegister
{
public:
    /**
    * @brief 注册模型/算法。
    *
    * @param[in] name    模型/算法的名称ID
    * @param[in] factory 模型/算法工厂类指针
    *
    */
    void                  registerModel(const string &name, ModelFactory* factory);

    /**
    * @brief 判断是否注册有指定名称的模型/算法。
    */
    bool                  hasModel(const string &name);

    /**
    * @brief 创建模型/算法指针。
    *
    * @param[in] name    模型/算法的名称ID
    * @return            模型/算法指针
    *
    */
    AppRunFlow*           create(const string &name);

    /**
    * @brief 获取注册机指针（静态方法）。
    * @return              注册机指针
    */
    static ModelRegister* getInstance();

private:
    map<string, shared_ptr<ModelFactory>> s_registers;  ///< 模型/算法注册表
};


template<class ModelType>
class AutoModelFactory: public ModelFactory
{
public:
    /**
    * @brief 模型自动工厂模版类，生成指定类型的模型/算法指针。
    * @return              模型/算法基类指针（实际指向子类）
    */
    AppRunFlow* create() override
    {
        return new ModelType();
    }
};


template<class ModelType>
class AutoModelRegister
{
public:
    /**
    * @brief 模型自动注册机，将指定名称的模型/算法添加到注册表。
    */
    AutoModelRegister(const string &name)
    {
        ModelRegister::getInstance()->registerModel(name, 
            new AutoModelFactory<ModelType>());
    }
};


#define MODEL_REGISTER(Name, Model) \
static AutoModelRegister<Model> autoRegister_##Name(Name);


#endif