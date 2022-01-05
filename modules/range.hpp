/** *****************************************************************************
*   @copyright :  Copyright (C) 2022 Qin ZhaoYu. All rights reserved.
*
*   @author    :  Qin ZhaoYu.
*   @see       :  https://github.com/QINZHAOYU
*   @brief     :  To implement a `range` function similar to Python.
*
*   Change History:
*   -----------------------------------------------------------------------------
*   v1.0, 2022/01/04, Qin ZhaoYu, zhaoyu.qin@foxmail.com
*   Init model.
*
** ******************************************************************************/
#pragma once
#include "common/CommHeader.hpp"


namespace ccb
{
/// \brief Iterator for custom range.
template<typename T>
class Iterator
{
public:
	using value_type = T;
	using size_type  = size_t;

private:
	size_type        _cursor;
	const value_type _step;
	value_type       _value;

public:
	Iterator(size_type currStart, value_type begValue, value_type stepValue)
		: _cursor(currStart)
		, _step(stepValue)
		, _value(begValue)
	{
		_value += (_step * _cursor);
	}
	value_type operator*()const
	{
		return _value;
	}

	bool operator!=(const Iterator &rhs)const
	{
		return (_cursor != rhs._cursor);
	}

	Iterator &operator++()
	{
		_cursor ++;
		_value += _step;
		return *this;
	}
};


/// \brief Return object of custom range.
template<typename T>
class Impl
{
public:
	using value_type      = T;
	using size_type       = size_t;
	using reference       = const value_type&;  // don't allow changes after construction.
	using const_reference = const value_type&;
	using iterator        = const Iterator<value_type>; // don't allow changes after construction.
	using const_iterator  = const Iterator<value_type>;

private:
	const value_type _begin;
	const value_type _end;
	const value_type _step;
	const size_type  _maxCount;

	size_type getAdjustedCount() const
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

public:
	Impl(value_type begValue, value_type endValue, value_type stepValue)
		: _begin(begValue)
		, _end(endValue)
		, _step(stepValue)
		, _maxCount(getAdjustedCount())
	{}

	size_type size()const
	{
		return _maxCount;
	}

	const_iterator begin()const
	{
		return {0, _begin, _step};
	}

	const_iterator end()const
	{
		return {_maxCount, _end, _step};
	}


};


/// \brief Custom range.
template<typename T>
Impl<T> Range(T end)
{
	return {{}, end, 1};
}

template<typename T>
Impl<T> Range(T begin, T  end)
{
	return {begin, end, 1};
}

template<typename T, typename U>
auto Range(T begin, T end, U step) -> Impl < decltype(step) >
{
	using r_t = Impl < decltype(step) >;
	return r_t(begin, end, step);
}


}