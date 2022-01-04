/** ***********************************************************************************
*    @File      :  range.inl
*    @Brief     :  To implement a `range` function similar to Python.
*
** ************************************************************************************/
#include "range.hpp"


// class iterator ---------------------------------------------------------------------

template<typename T>
iterator<T>::iterator(size_type currStart, value_type begValue, value_type stepValue)
	: _cursor(currStart)
	, _step(stepValue)
	, _value(begValue)
{
	_value += (_step * _cursor);
}

template<typename T>
iterator<T>::value_type iterator<T>::operator*()const
{
	return _value;
}

template<typename T>
bool iterator<T>::operator!=(const iterator &rhs)const
{
	return (_cursor != rhs._cursor);
}

template<typename T>
iterator<T> &iterator<T>::operator++()
{
	_cursor ++;
	_value += _step;
	return *this;
}


// class impl -------------------------------------------------------------------------

template<typename T>
impl<T>::impl(value_type begValue, value_type endValue, value_type stepValue)
	: _begin(begValue)
	, _end(endValue)
	, _step(stepValue)
	, _maxCount(getAdjustedCount())
{}

template<typename T>
impl<T>::size_type impl<T>::getAdjustedCount() const
{
	if (_step > 0 && _begin >= _end)
	{
		throw std::logic_error("End value must be greater than begin value.");
	}
	else if (_step < 0 && _begin <= _end)
	{
		throw std::logic_error("End Value must be less than begin value.")
	}

	size_type x = static_cast<size_type>((_end - _begin) / _step);
	if (_begin + (_step * x) != _end) { ++x; }

	return x;
}

template<typename T>
impl<T>::size_type impl<T>::size()const
{
	return _maxCount;
}

template<typename T>
impl<T>::const_iterator impl<T>::begin()const
{
	return {0, _begin, _step};
}

template<typename T>
impl<T>::const_iterator impl<T>::end()const
{
	return {_maxCount, _end, _step};
}

