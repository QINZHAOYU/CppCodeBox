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

class AppFunFlow;


class ModelFactory
{
public:
    virtual AppFunFlow* create()=0;
};


class ModelRegister
{
public:
    void registerModel(const string &name, ModelFactory* factory);
    bool hasModel(const string &name);
    AppFunFlow* create(const string &name);
    static ModelRegister* getInstance();

private:
    map<string, shared_ptr<ModelFactory>> s_registers;
};


template<class ModelType>
class AutoModelFactory: public ModelFactory
{
public:
    AppFunFlow* create() override
    {
        return new ModelType();
    }
};


template<class ModelType>
class AutoModelRegister
{
public:
    AutoModelRegister(const string &name)
    {
        ModelRegister::getInstance()->registerModel(name, 
            new ModelFactory<ModelType>);
    }
};


#define MODEL_REGISTER(Name, Model) \
static AutoModelRegister<Model> autoRegister_##Name(Name);


#endif