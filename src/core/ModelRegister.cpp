/** *****************************************************************************
*    @File      :  ModelRegister.cpp
*    @Brief     :  模块注册器实现。
*
** ******************************************************************************/
#include "ModelRegister.h"


void ModelRegister::registerModel(const string &name, ModelFactory* factory)
{
    s_registers.insert(make_pair(name, factory));
}

bool ModelRegister::hasModel(const string &name)
{
    return s_registers.find(name) != s_registers.end();
}

AppFunFlow* ModelRegister::create(const string &name)
{
    auto factory = s_registers.find(name);
    if(factory != s_registers.end()){
        return factory->second->create();
    }        
    
    return nullptr;
}

ModelRegister* ModelRegister::getInstance()
{
    static ModelRegister mReg;  //静态变量。
    return &mReg;
}


