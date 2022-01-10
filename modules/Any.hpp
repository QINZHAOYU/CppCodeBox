/** *****************************************************************************
*   @copyright :  Copyright (C) 2022 Qin ZhaoYu. All rights reserved.
*
*   @author    :  Qin ZhaoYu.
*   @see       :  https://github.com/QINZHAOYU
*   @brief     :  To implement a `Any` class.
*
*   Change History:
*   -----------------------------------------------------------------------------
*   v1.0, 2022/01/10, Qin ZhaoYu, zhaoyu.qin@foxmail.com
*   Init model.
*
** ******************************************************************************/
#pragma once
#include "common/CommHeader.hpp"
#include <typeindex>


namespace ccb
{
class Any
{
public:
    Any(): _tpIndex(std::type_index(typeid(void))) 
    {}
    Any(const Any &that): _ptr(that.Clone()), _tpIndex(that._tpIndex) 
    {}
    Any(Any &&that): _ptr(std::move(that._ptr)), _tpIndex(that._tpIndex)
    {}

    // enable_if<bool, type>::type, get the type if condition is true;
    // is_same<type1, type2>::value, check if the two type are exactly the same;
    // decay<type>::type, remove the cv and reference of a type.
    template<typename U, class=typename std::enable_if<!std::is_same<typename std::decay<U>::type, Any>::value, U>::type> 
    Any(U &&value): 
    _ptr(new Derived<typename std::decay<U>::type> (std::forward<U>(value))), 
    _tpIndex(std::type_index(typeid(typename std::decay<U>::type)))
    {}

    bool IsNull() const
    {
        return !bool(_ptr);
    }

    template<class U>
    bool Is() const
    {
        return _tpIndex == std::type_index(typeid(U));
    }

    template<class U>
    U& AnyCast()  // type transformation(Downcast).
    {
        if (!Is<U>())
        {
            cout << "cant cast " << typeid(U).name() << " to " << _tpIndex.name() << endl;
            throw std::bad_cast();
        }

        auto derived = dynamic_cast<Derived<U>*> (_ptr.get());
        return derived->_value;
    }

    Any& operator=(const Any &a)
    {
        if (_ptr != a._ptr)
        {
            _ptr = a.Clone();
            _tpIndex = a._tpIndex;
        }

        return *this;
    }

private:
    class Base;
    using BasePtr = std::unique_ptr<Base>;

    /// \brief The base class used to earse the type infomation.
    class Base
    {
    public:
        virtual~Base(){}
        virtual BasePtr Clone() const = 0;
    };

    /// \brief The derived class used to obtain type in runtime. 
    template<typename T>
    class Derived : public Base
    {
    public:
        template<typename U>
        Derived(U &&value): _value(std::forward<U>(value))
        {}

        BasePtr Clone() const
        {
            return BasePtr(new Derived<T>(_value));
        }
    
    public:
        T _value;
    };

    BasePtr Clone() const
    {
        if (_ptr != nullptr)
        {
            return _ptr->Clone();
        }
        else
        {
            return nullptr;
        }
    }

private:
    BasePtr _ptr;  // base class pointer used to erase concrete type.
    std::type_index _tpIndex;  // type_index to save type infomation.
};

}

