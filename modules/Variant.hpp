/** *****************************************************************************
*   @copyright :  Copyright (C) 2021 Qin ZhaoYu. All rights reserved.
*
*   @author    :  Qin ZhaoYu.
*   @brief     :  To implement class Variant.
*
*   Change History:
*   -----------------------------------------------------------------------------
*   --version-1.0, 2021/10/11, Qin ZhaoYu,
*   To init model.
*
** ******************************************************************************/
#pragma once
#include "common/CommHeader.hpp"
#include "modules/FunctionTraits.hpp"
#include <typeindex>


namespace ccb
{
/// \brief To get maximal integer.
template<size_t arg, size_t... rest>
class MaxInteger;

template<size_t arg>
class MaxInteger<arg> : public std::integral_constant<size_t, arg>
{};

template<size_t arg1, size_t arg2, size_t... rest>
class MaxInteger<arg1, arg2, rest...> : public std::integral_constant<size_t,
( arg1 >= arg2 )
? MaxInteger<arg1, rest...>::value
: MaxInteger<arg2, rest...>::value> 
{};


/// \brief To get maximal `align`.
template<typename... Args>
class MaxAlign : public std::integral_constant<int, 
MaxInteger<std::alignment_of<Args>::value...>::value>
{};


/// \brief To check if containing type.
template<typename T, typename... Lists>
class Contains : public std::true_type
{};

template<typename T, typename Head, typename... Rest>
class Contains<T, Head, Rest...>: public std::conditional<
std::is_same<T, Head>::value, 
std::true_type, Contains<T, Rest...>>::type
{};

template<typename T>
class Contains<T> : public std::false_type
{};


/// \brief To get index of given type.
template<typename T, typename... Lists>
class IndexOf;

template<typename T, typename Head, typename... Rest>
class IndexOf<T, Head, Rest...>
{
public:
    enum { value = IndexOf<T, Rest...>::value + 1 };   
};

template<typename T, typename... Rest>
class IndexOf<T, T, Rest...>
{
    enum { value = 0 };
};

template<typename T>
class IndexOf<T>
{
    enum { value = -1 };
};


/// \brief To get type of given index.
template<int Index, typename... Types>
class At;

template<int Index, typename First, typename... Types>
class At<Index, First, Types...>
{
    using type = typename At<Index-1, Types...>::type;
};

template<typename T, typename... Types>
class At<0, T, Types...>
{
    using type = T;
};


/// \brief Variant helper.
/// This is not necessary now, for its functiona have been implemented in Variant.
template<typename... Args>
class VariantHelper;

template<typename T, typename... Args>
class VariantHelper<T, Args...> 
{
	inline static void Destroy(std::type_index id, void * data)
	{
		if (id == std::type_index(typeid(T)))
			reinterpret_cast<T*>(data)->~T();
		else
			VariantHelper<Args...>::Destroy(id, data);
	}

	inline static void move(std::type_index old_t, void * old_v, void * new_v)
	{
		if (old_t == std::type_index(typeid(T)))
			new (new_v) T(std::move(*reinterpret_cast<T*>(old_v)));
		else
			VariantHelper<Args...>::move(old_t, old_v, new_v);
	}

	inline static void copy(std::type_index old_t, const void * old_v, void * new_v)
	{
		if (old_t == std::type_index(typeid(T)))
			new (new_v) T(*reinterpret_cast<const T*>(old_v));
		else
			VariantHelper<Args...>::copy(old_t, old_v, new_v);
	}
};

template<>
class VariantHelper<>  
{
	inline static void Destroy(std::type_index id, void * data) {  }
	inline static void move(std::type_index old_t, void * old_v, void * new_v) { }
	inline static void copy(std::type_index old_t, const void * old_v, void * new_v) { }
};


/// \brief Variant used to take place of union.
template<typename... Types>
class Variant
{
public:
    template<int Index>
    using IndexType = typename At<Index, Types...>::type;

    Variant() : _typeIndex(typeid(void))
    {}
    Variant(Variant<Types...> &&old) : _typeIndex(old._typeIndex)
    {
        Move(old._typeIndex, &old._data, &_data);
    }
    Variant(const Variant<Types...> &old) : _typeIndex(old._typeIndex)
    {
        Copy(old._typeIndex, &old._data, &_data);
    } 
    ~Variant()
    {
        Destroy(_typeIndex, &_data);
    }

    template<typename T, class=typename std::enable_if<Contains<
    typename std::decay<T>::type, Types...>::value>::type>
    Variant(T &&value) : _typeIndex(typeid(void))
    {
        Destroy(_typeIndex, &_data);

        using U = typename std::decay<T>::type;
        new (&_data) U(std::Forward<T>(value));
        _typeIndex = std::type_index(typeid(U));
    }

    Variant& operator=(const Variant &old)
    {
        Copy(old._typeIndex, &old._data, &_data);
        _typeIndex = old._typeIndex;
        return *this;
    }
    Variant& operator=(Variant &&old)
    {
        Move(old._typeIndex, &old._data, &_data);
        _typeIndex = old._typeIndex;
        return *this;
    }

    template<typename T>
    bool Is() const
    {
        return _typeIndex == std::type_index(typeid(T));
    }

    bool Empty() const
    {
        return _typeIndex == std::type_index(typeid(void));
    }

    std::type_index Type() const
    {
        return _typeIndex;
    }

    template<typename T>
    typename std::decay<T>::type& Get()
    {
        using U = typename std::decay<T>::type;
        if (!Is<U>())
        {
            cout << typeid(U).name() << " not defined.\t";
            cout << "current type: " << _typeIndex.name();
            cout << endl;

            throw std::bad_cast();
        }

        return *(U*)(_data);
    }

    template<typename T>
    int indexOf()
    {
        return IndexOf<T, Types...>::value;
    }

    bool operator==(const Variant &rhs) const
    {
        return _typeIndex == rhs._typeIndex;
    }

    bool operator<(const Variant &rhs) const
    {
        return _typeIndex < rhs._typeIndex;
    }

	template<typename F>
	void Visit(F&& f)
	{
		using T = typename function_traits<F>::args<0>::type;
		if (Is<T>())
			f(Get<T>());
	}

	template<typename F, typename... Rest>
	void Visit(F&& f, Rest&&... rest)
	{
		using T = typename function_traits<F>::arg_type<0>;
		if (Is<T>())
			Visit(std::forward<F>(f));
		else
			Visit(std::forward<Rest>(rest)...);
	}


private:
    void Destroy(const std::type_index &index, void *buff)
    {
        [](Types&&...){}((Destroy0<Types>(index, buff), 0)...);
    }

    template<typename T>
    void Destroy0(const std::type_index &id, void *data)
    {
        if (id == std::type_index(typeid(T)))
        {
            reinterpret_cast<T*>(data)->~T();
        }
    }

    void Move(const std::type_index &old_t, void *old_v, void *new_v)
    {
        [](Types&&...){} ((Move0(old_t, old_v, new_v), 0)...);
    }

    template<typename T>
    void Move0(const std::type_index &old_t, void *old_v, void *new_v)
    {
        if (old_t == std::type_index(typeid(T)))
        {
            new (new_v) T(std::move(*reinterpret_cast<T*>(old_v)));
        }
    }

    void Copy(const std::type_index &old_t, const void *old_v, void *new_v)
    {
        [](Types&&...){} ((Copy0(old_t, old_v, new_v), 0)...);
    }

    template<typename T>
    void Copy0(const std::type_index &old_t, const void *old_v, void *new_v)
    {
        if (old_t == std::type_index(typeid(T)))
        {
            new (new_v) T(*reinterpret_cast<const T*>(old_v));
        }
    }

private:
    enum 
    {
        data_size = MaxInteger<sizeof(Types)...>::value,
        align_size = MaxAlign<Types...>::value,
    };

    using data_t = typename std::aligned_storage<data_size, align_size>::type;
    using helper_t = typename VariantHelper<Types...>;

    data_t _data;
    std::type_index _typeIndex;
};




}

