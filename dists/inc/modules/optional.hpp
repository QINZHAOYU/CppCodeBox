/** *****************************************************************************
*   @copyright :  Copyright (C) 2021 Qin ZhaoYu. All rights reserved.
*
*   @author    :  Qin ZhaoYu.
*   @brief     :  To implement class Optional similar to boost::optional.
*
*   Change History:
*   -----------------------------------------------------------------------------
*   --version-1.0, 2021/10/09, Qin ZhaoYu,
*   To init model.
*
** ******************************************************************************/
#pragma once

#include "common/CommHeader.hpp"
#include <type_traits>
#include <exception>


template<typename T>
class Optional
{
private:
    using data_t = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

public:
    Optional() {}
    Optional(const T& v)
    {
        create(v);
    }
    Optional(const Optional& other)
    {
        if (other.isInit())
        {
            assign(other);
        }
    }
    Optional& operator=(const Optional& other)
    {
        if (other.isInit())
        {
            assign(other);
            return *this;
        }

        throw logic_error("is not valid.");
    }
    Optional& operator=(const T& val)
    {
        _hasInit = true;
        destroy();
        new (&_data) T(*((T*)(&val)));
        return *this;
    }

    ~Optional()
    {
        destroy();
    }

    template<class... Args>
    void emplace(Args&&... args)
    {
        destroy();
        create(forward<Args>(args)...);
    }

    bool isInit() const
    {
        return _hasInit;
    }

    explicit operator bool() const
    {
        return isInit();
    }

    T& operator *() const
    {
        if (isInit())
        {
            return *((T*) (&_data));
        }
        throw logic_error("is not inited.");
    }

private:
    template<class... Args>
    void create(Args&&... args)
    {
        new (&_data) T(forward<Args>(args)...);
        _hasInit = true;
    }

    void destroy()
    {
        if (_hasInit)
        {
            _hasInit = false;
            ((T*) (&_data))->~T();
        }
    }

    void assign(const Optional& other)
    {
        if (other.isInit())
        {
            _hasInit = true;
            copy(other._data);
        }
        else
        {
            destroy();
        }
    }

    void copy(const data_t& val)
    {
        destroy();
        new (&_data) T(*((T*)(&val)));
    }

private:
    bool _hasInit{false};
    data_t _data;
};


