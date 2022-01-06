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
Optional<T>::Optional(const Optional &other)
{
	if (other.isInit())
	{
		assign(other);
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
	throw logic_error("is not valid.");
}

template<typename T>
Optional<T> &Optional<T>::operator=(const T &val)
{
	_hasInit = true;
	destroy();
	new (&_data) T(*((T *)(&val)));
	return *this;
}

template<typename T>
template<class... Args>
void Optional<T>::emplace(Args &&... args)
{
	destroy();
	create(forward<Args>(args)...);
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
T &Optional<T>::operator *() const
{
	if (isInit())
	{
		return *((T *) (&_data));
	}
	throw logic_error("is not inited.");
}

template<typename T>
template<class... Args>
void Optional<T>::create(Args &&... args)
{
	new (&_data) T(forward<Args>(args)...);
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
		_hasInit = true;
		copy(other._data);
	}
	else
	{
		destroy();
	}
}

template<typename T>
void Optional<T>::copy(const data_t &val)
{
	destroy();
	new (&_data) T(*((T *)(&val)));
}