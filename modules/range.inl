/** ***********************************************************************************
*    @File      :  range.inl
*    @Brief     :  To implement a `range` function similar to Python.
*
** ************************************************************************************/
#include "range.hpp"

using namespace ccb;


// class Iterator ---------------------------------------------------------------------

template<typename T>
Iterator<T>::Iterator(size_type currStart, value_type begValue, value_type stepValue)
	: _cursor(currStart)
	, _step(stepValue)
	, _value(begValue)
{
	_value += (_step * _cursor);
}

template<typename T>
T Iterator<T>::operator*()const
{
	return _value;
}

template<typename T>
bool Iterator<T>::operator!=(const Iterator &rhs)const
{
	return (_cursor != rhs._cursor);
}

template<typename T>
Iterator<T> &Iterator<T>::operator++()
{
	_cursor ++;
	_value += _step;
	return *this;
}


// class Impl -------------------------------------------------------------------------

template<typename T>
Impl<T>::Impl(value_type begValue, value_type endValue, value_type stepValue)
	: _begin(begValue)
	, _end(endValue)
	, _step(stepValue)
	, _maxCount(getAdjustedCount())
{}

template<typename T>
size_t Impl<T>::getAdjustedCount() const
{
	if (_step > 0 && _begin >= _end)
	{
		throw std::logic_error("End value must be greater than begin value.");
	}
	else if (_step < 0 && _begin <= _end)
	{
		throw std::logic_error("End Value must be less than begin value.");
	}

	size_type x = static_cast<size_type>((_end - _begin) / _step);
	if (_begin + (_step * x) != _end) { ++x; }

	return x;
}

template<typename T>
size_t Impl<T>::size()const
{
	return _maxCount;
}

template<typename T>
const Iterator<T> Impl<T>::begin() const
{
	return {0, _begin, _step};
}

template<typename T>
const Iterator<T> Impl<T>::end()const
{
	return {_maxCount, _end, _step};
}

