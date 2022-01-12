/** *****************************************************************************
*   @copyright :  Copyright (C) 2022 Qin ZhaoYu. All rights reserved.
*
*   @author    :  Qin ZhaoYu.
*   @brief     :  To implement a scope guard to keep resource released.
*
*   Change History:
*   -----------------------------------------------------------------------------
*   --version-1.0, 2022/01/12, Qin ZhaoYu,
*   To init model.
*
** ******************************************************************************/
#pragma once
#include "common/CommHeader.hpp"


namespace ccb
{
template<typename F>
class ScopeGuard
{
public:
    explicit ScopeGuard(F &&f) : _func(std::move(f)), _dismiss(false)
    {}
    explicit ScopeGuard(const F &f) : _func(f), _dismiss(false)
    {}

    ScopeGuard(ScopeGuard &&rhs) : _func(std::move(rhs._func)), _dismiss(rhs._dismiss)
    {
        rhs.Dismiss();
    }

    ~ScopeGuard()
    {
        if (!_dismiss && _func != nullptr)
        {
            _func();
        }
    }

    void Dismiss()
    {
        _dismiss = true;
    }

private:
    ScopeGuard();
    ScopeGuard(const ScopeGuard &);
    ScopeGuard& operator=(const ScopeGuard &);

private:
    F _func;
    bool _dismiss;
};


template<typename F>
ScopeGuard<typename std::decay<F>::type> MakeGuard(F &&f)
{
    using scope_t = typename std::decay<F>::type;
    return ScopeGuard<scope_t>(std::forward<F>(f));
}


}

