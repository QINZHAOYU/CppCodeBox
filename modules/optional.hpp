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


namespace ccb
{
template<typename T>
class Optional
{
private:
	using data_t = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

	bool   _hasInit{false};
	data_t _data;

public:
	Optional();
	Optional(const T &v);
	Optional(const Optional &other);
	~Optional();	

	Optional &operator=(const Optional &other);
	Optional &operator=(const T &val);

	template<class... Args>
	void emplace(Args &&... args);

	bool isInit() const;

	explicit operator bool() const;
	T &operator *() const;

private:
	template<class... Args>
	void create(Args &&... args);

	void destroy();

	void assign(const Optional &other);

	void copy(const data_t &val);


};
#include "optional.inl"

}

