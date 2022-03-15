#pragma once
#include "Types.h"

#pragma region TypeCast

template <typename ...T>
struct TypeList;

template<typename T, typename U>
struct TypeList<T, U>
{
	using Head = T;
	using Tail = U;
};

template<typename T, typename... U>
struct TypeList<T, U...>
{
	using Head = T;
	using Tail = TypeList<U...>;
};

#pragma endregion

#pragma region Length
template <typename T>
struct Length;

template <>
struct Length<TypeList<>>
{
	enum { value = 0 };
};

template <typename T, typename... U>
struct Length<TypeList<T, U...>>
{
	enum { value = 1 + Length<TypeList<U...>>::value };
};

#pragma endregion

#pragma region TypeAt
template<typename TL, int32 index>
struct TypeAt;

template<typename T, typename... U>
struct TypeAt<TypeList<T, U...>, 0>
{
	using Result = T;
};

template<typename T, typename...U, int32 index>
struct TypeAt<TypeList<T, U...>, index>
{
	using Result = typename TypeAt<TypeList<U...>, index - 1>::Result;
};
#pragma endregion

#pragma region IndexOf
template<typename TL, typename T>
struct IndexOf;

template<typename...T, typename U>
struct IndexOf<TypeList<U, T...>, U>
{
	enum
	{
		value = 0
	};
};
template<typename T>
struct IndexOf<TypeList<>, T>
{
	enum
	{
		value = -1
	};
};

template<typename X, typename...T, typename U>
struct IndexOf<TypeList<X, T...>, U>
{
private:
	enum { temp = IndexOf<TypeList<T...>, U>::value };
public:
	enum { value = (temp == -1) ? -1 : temp + 1 };
};

#pragma endregion

#pragma region Conversion
template<typename From, typename To>
class Conversion
{
private:
	using Small = __int8;
	using Big	= __int32;

	static Small	Test(const To&) { return 0; }
	static Big		Test(...) { return 0; }
	static From		MakeFrom() { return 0; }

public:
	enum
	{
		exists = sizeof(Test(MakeFrom())) == sizeof(Small),
	};
};
#pragma endregion

#pragma region Int2Type
template <int32 V>
struct Int2Type
{
	enum { value = V };
};
#pragma endregion

#pragma region TypeConversion
template<typename TL>
class TypeConversion
{
public:
	enum { length = Length<TL>::value };

	TypeConversion()
	{

		MakeTable(Int2Type<0>(), Int2Type<0>());
	}

	template<int32 i, int32 j>
	static void MakeTable(Int2Type<i>, Int2Type<j>)
	{
		using FromType = typename TypeAt<TL, i>::Result;
		using ToType = typename TypeAt<TL, j>::Result;
		if (Conversion<FromType, ToType>::exists) { s_convert[i][j] = true; }
		else { s_convert[i][j] = false; }

		MakeTable(Int2Type<i>(), Int2Type<j + 1>());
	}
	
	template<int32 i>
	static void MakeTable(Int2Type<i>, Int2Type<length>)
	{
		MakeTable(Int2Type<i + 1>(), Int2Type<0>());
	}
	
	template<int32 j>
	static void MakeTable(Int2Type<length>, Int2Type<j>) { }

	static bool CanConvert(int32 from, int32 to)
	{
		static TypeConversion conversion;
		// 안 만들어 졌을 수가 있으므로 static으로 생성자가 호출되도록 한다
		return s_convert[from][to];
	}

public:
	static bool s_convert[length][length];
};

template<typename TL>
bool TypeConversion<TL>::s_convert[length][length];


template<typename To, typename From>
To TypeCast(From* ptr)
{
	if (ptr == nullptr) return nullptr;

	using TL = typename From::TL;

	if (TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, std::remove_pointer_t<To>>::value))
		return static_cast<To>(ptr);

	return nullptr;
}

template<typename To, typename From>
std::shared_ptr<To> TypeCast(std::shared_ptr<From> ptr)
{
	if (ptr == nullptr) return nullptr;

	using TL = typename From::TL;

	if (TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, std::remove_pointer_t<To>>::value))
		return std::static_pointer_cast<To>(ptr);

	return nullptr;
}

template<typename To, typename From>
bool CanCast(From* ptr)
{
	if (ptr == nullptr) return false;

	using TL = typename From::TL;
	return TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, std::remove_pointer_t<To>>::value);
}

template<typename To, typename From>
bool CanCast(std::shared_ptr<From> ptr)
{
	if (ptr == nullptr) return false;

	using TL = typename From::TL;
	return TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, std::remove_pointer_t<To>>::value);
}
#pragma endregion

#define DECLARE_TL		using TL = TL;	int32 _typeId;
#define INIT_TL(Type)	_typeId = IndexOf<TL, Type>::value;