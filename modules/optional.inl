/** *******************************************************************************
*    @File      :  optional.inl
*    @Brief     :  None.
*
** ********************************************************************************/
#include "optional.hpp"

using namespace ccb;


template<typename T>
Optional<T>::Optional() 
{}

template<typename T>
Optional<T>::Optional(const T &v)
{
	create(v);
}

template<typename T>
Optional<T>::Optional(T &&v)
{
	create(std::move(v));
}

template<typename T>
Optional<T>::Optional(const Optional &other)
{
	if (other.isInit())
	{
		assign(other);
	}
}

template<typename T>
Optional<T>::Optional(Optional &&other)
{
	if (other.isInit())
	{
		assign(std::move(other));
        other.destroy();
	}
}

template<typename T>
Optional<T>::~Optional()
{
	destroy();
}

template<typename T>
Optional<T> &Optional<T>::operator=(const Optional &other)
{
	if (other.isInit())
	{
		assign(other);
		return *this;
	}
	throw std::logic_error("is not valid.");
}

template<typename T>
Optional<T> &Optional<T>::operator=(Optional &&other)
{
    assign(std::move(other));
    return *this;
}

template<typename T>
template<class... Args>
void Optional<T>::emplace(Args &&... args)
{
	destroy();
	create(std::forward<Args>(args)...);
}

template<typename T>
bool Optional<T>::isInit() const
{
	return _hasInit;
}

template<typename T>
Optional<T>::operator bool() const
{
	return isInit();
}

template<typename T>
T &Optional<T>::operator *()
{
    return *((T *) (&_data));  // get the value;
}

template<typename T>
T const& Optional<T>::operator *() const
{
	if (isInit())
	{
		return *((T *) (&_data));
	}
	throw std::logic_error("is not inited.");
}

template<typename T>
bool Optional<T>::operator == (const Optional<T>& rhs) const
{
	// first, check if initialization status;
	// if same, check the value.
    return (!bool(*this)) != (!rhs) 
	? false 
	: (!bool(*this) ? true : (*(*this)) == (*rhs));
}

template<typename T>
bool Optional<T>::operator < (const Optional<T>& rhs) const
{
	bool isLess = (!rhs)
    ? false 
    : ( !bool(*this) ? true : (*(*this) < (*rhs)) );

    return isLess;
}

template<typename T>
bool Optional<T>::operator != (const Optional<T>& rhs)
{
	return !(*this == (rhs));
}


template<typename T>
template<class... Args>
void Optional<T>::create(Args &&... args)
{
	new (&_data) T(std::forward<Args>(args)...);
	_hasInit = true;
}

template<typename T>
void Optional<T>::destroy()
{
	if (_hasInit)
	{
		_hasInit = false;
		((T *) (&_data))->~T();
	}
}

template<typename T>
void Optional<T>::assign(const Optional &other)
{
	if (other.isInit())
	{
		copy(other._data);        
		_hasInit = true;
	}
	else
	{
		destroy();
	}
}

template<typename T>
void Optional<T>::assign(Optional &&other)
{
	if (other.isInit())
	{
		move(std::move(other._data));
        _hasInit = true;
        other.destroy();
	}
	else
	{
		destroy();
	}
}

template<typename T>
void Optional<T>::move(data_t&& val)
{
	destroy();
	new (&_data) T(std::move(*((T*)(&val))));
}

template<typename T>
void Optional<T>::copy(const data_t &val)
{
	destroy();
	new (&_data) T(*((T *)(&val)));
}