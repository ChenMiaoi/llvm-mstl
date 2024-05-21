//
// Created by dezi on 5/19/2024.
//

#ifndef LLVM_MSTL_COMPRESS_PAIR_H
#define LLVM_MSTL_COMPRESS_PAIR_H

#include "__config.h"
#include "__tuple_dir/tuple_indices.h"
#include "__type_traits/dependent_type.h"
#include "__utility/integer_sequence.h"

#include <type_traits>
#include <utility>

LLVM_MSTL_BEGIN_NAMESPACE_STD

LLVM_MSTL_CORE_STD

/**
 * @brief Tag type for default initialization.
 */
struct __default_init_tag {};

/**
 * @brief Tag type for value initialization.
 */
struct __value_init_tag {};

/**
 * @brief Element of the compressed pair.
 * 
 * This structure template represents an element within the compressed pair.
 * The behavior of the element depends on the template parameters _Tp and _Idx.
 * It either contains a value of type _Tp or, if _CanBeEmptyBase is true and _Tp is an empty type that cannot be inherited, it inherits from _Tp.
 *
 */
template < typename _Tp, int32_t _Idx, bool _CanBeEmptyBase = core::is_empty_v< _Tp >&& __cpp_lib_is_final >
struct __compressed_pair_elem {
	using _ParamT         = _Tp;
	using reference       = _Tp&;
	using const_reference = const _Tp&;

	/**
		* @brief Default constructor with __default_init_tag.
		*/
	LLVM_MSTL_CONSTEXPR explicit __compressed_pair_elem( __default_init_tag ) = default;
	/**
		* @brief Value constructor with __value_init_tag.
		*/
	LLVM_MSTL_CONSTEXPR explicit __compressed_pair_elem( __value_init_tag )
			: __value__() {}

	/**
		* @brief Value constructor with forwarding reference.
		* @tparam _Up The type of the argument.
		* @param __u The argument to be forwarded.
		*/
	template <
		typename _Up,
		typename = core::enable_if_t<
			!core::is_same_v< __compressed_pair_elem, typename core::decay_t< _Up > > > >
	LLVM_MSTL_CONSTEXPR explicit __compressed_pair_elem( _Up&& __u )
			: __value__( core::forward< _Up >( __u ) ) {}

	/**
		* @brief Piecewise constructor with tuple indices.
		* @tparam _Args The types of the arguments.
		* @tparam _Indices The indices of the tuple elements.
		* @param __args The tuple of arguments.
		* @param __tuple_indices The indices of the tuple elements.
		*/
	template < typename... _Args, size_t... _Indices >
	LLVM_MSTL_CONSTEXPR explicit __compressed_pair_elem(
		core::piecewise_construct_t, core::tuple< _Args... > __args, __tuple_indices< _Indices... > )
			: __value__( core::forward< _Args >( core::get< _Indices >( __args ) )... ) {}

	/**
		* @brief Returns a reference to the stored value.
		* @return A reference to the stored value.
		*/
	LLVM_MSTL_CONSTEXPR auto __get() LLVM_MSTL_NOEXCEPT->reference {
		return __value__;
	}

	/**
		* @brief Returns a const reference to the stored value.
		* @return A const reference to the stored value.
		*/
	LLVM_MSTL_CONSTEXPR auto __get() const LLVM_MSTL_NOEXCEPT->const_reference {
		return __value__;
	}

private:
	_Tp __value__;//<--- The stored value.
};

template < typename _Tp, int32_t _Idx >
struct __compressed_pair_elem< _Tp, _Idx, true > : private _Tp {
	using _ParamT         = _Tp;
	using reference       = _Tp&;
	using const_reference = const _Tp&;
	using __value_type    = _Tp;

	LLVM_MSTL_CONSTEXPR explicit __compressed_pair_elem()                     = default;
	LLVM_MSTL_CONSTEXPR explicit __compressed_pair_elem( __default_init_tag ) = default;
	LLVM_MSTL_CONSTEXPR explicit __compressed_pair_elem( __value_init_tag )
			: __value_type() {}

	template <
		typename _Up,
		typename = core::enable_if_t<
			!core::is_same_v< __compressed_pair_elem, typename core::decay_t< _Up > > > >
	LLVM_MSTL_CONSTEXPR explicit __compressed_pair_elem( _Up&& __u )
			: __value_type( core::forward< _Up >( __u ) ) {}


	template < typename... _Args, size_t... _Indices >
	LLVM_MSTL_CONSTEXPR explicit __compressed_pair_elem(
		core::piecewise_construct_t, core::tuple< _Args... > __args, __tuple_indices< _Indices... > )
			: __value_type( core::forward< _Args >( core::get< _Indices >( __args ) )... ) {}

	LLVM_MSTL_CONSTEXPR auto __get() LLVM_MSTL_NOEXCEPT->reference {
		return *this;
	}

	LLVM_MSTL_CONSTEXPR auto __get() const LLVM_MSTL_NOEXCEPT->const_reference {
		return *this;
	}
};

/**
 * @brief Compressed pair class template. `__compressed_pair` is designed to store two types of values by minimizing memory usage.
 *
 * @note class template represents a compressed pair that can store values of two different types.
 *
 * @note '__compressed_pair' provides several constructors for different initialization methods,
 * including default construction, value initialization, and tuple initialization.
 * 
 * @note It also provides the 'first()' and 'second()' member functions for accessing the two stored values.
 * In addition, the 'swap()' function swaps the values of two '__compressed_pair' objects.
 *
 * @note "空基类优化"（Empty Base Optimization）是一种优化技术，用于在某些情况下减少空基类的存储开销。
 * 在C++中，如果派生类有一个或多个基类，而这些基类中至少有一个是空类（没有数据成员），那么通常情况下每个基类都会占用一定的存储空间。这是因为每个对象都需要有独立的地址，以确保成员的访问和布局的正确性。
 * 然而，对于空类的特殊情况，C++标准允许编译器进行优化，即不分配额外的存储空间给空类的实例。这样做的原因是，空类没有数据成员，因此它们的实例不需要独立的地址。
 * 空基类优化的实现方式是将派生类对象的基类子对象与派生类的数据成员放在同一块内存中，以节省存储空间。这样，派生类的对象在内存中的布局就不会包含额外的空基类存储空间。
 * 使用空基类优化可以在一定程度上减小数据结构的大小，特别是当派生类有多个空基类时，优化效果更为明显。然而，空基类优化的可行性取决于编译器的实现和对特定情况的支持。
 * 需要注意的是，空基类优化只适用于空类，并不适用于带有虚函数或非空成员的基类。在这些情况下，基类仍然会分配独立的存储空间。
 * 
 *
 * @tparam _T1 The first type.
 * @tparam _T2 The second type.
 */
template < typename _T1, typename _T2 >
class __compressed_pair : private __compressed_pair_elem< _T1, 0 >,
													private __compressed_pair_elem< _T2, 1 > {
public:
	static_assert(
		core::is_same_v< _T1, _T1 >,
		"__compressed_pair cannot be instantiated when T1 and T2 are the same type; "
		"The current implementation is NOT ABI-compatible with the previous implementation for this configuration" );

	using _Base1 = __compressed_pair_elem< _T1, 0 >;//<--- The base element for the first type.
	using _Base2 = __compressed_pair_elem< _T2, 1 >;//<--- The base element for the second type.

	/**
		* @brief Value constructor.
		*
		* Constructs a compressed pair with the given values for both types.
		*
		* @tparam _U1 The type of the first argument.
		* @tparam _U2 The type of the second argument.
		* @brief Default constructor.
		*
		* Constructs a compressed pair with default-initialized values for both types.
		*/
	template <
		bool _Dummy = true,
		typename    = core::enable_if_t<
      __dependent_type< core::is_default_constructible< _T1 >, _Dummy >::value &&
      __dependent_type< core::is_default_constructible< _T2 >, _Dummy >::value > >
	LLVM_MSTL_CONSTEXPR explicit __compressed_pair()
			: _Base1( __value_init_tag() )
			, _Base2( __value_init_tag() ) {}

	/**
		* @brief Value constructor.
		*
		* Constructs a compressed pair with the given values for both types.
		*
		* @tparam _U1 The type of the first argument.
		* @tparam _U2 The type of the second argument.
		* @param __t1 The value for the first type.
		* @param __t2 The value for the second type.
		*/
	template < typename _U1, typename _U2 >
	LLVM_MSTL_CONSTEXPR explicit __compressed_pair( _U1&& __t1, _U2&& __t2 )
			: _Base1( core::forward< _U1 >( __t1 ) )
			, _Base2( core::forward< _U2 >( __t2 ) ) {}

	/**
		* @brief Piecewise constructor with tuples for initializing the compressed pair.
		*
		* Constructs a compressed pair using two tuples of arguments to initialize the first and second types separately.
		*
		* @tparam _Args1 The types of the arguments for the first type.
		* @tparam _Args2 The types of the arguments for the second type.
		* @param __pc The piecewise construct tag.
		* @param __first_args The tuple of arguments for the first type.
		* @param __second_args The tuple of arguments for the second type.
		*/
	template < typename... _Args1, typename... _Args2 >
	LLVM_MSTL_CONSTEXPR explicit __compressed_pair(
		core::piecewise_construct_t __pc,
		core::tuple< _Args1... >    __first_args,
		core::tuple< _Args2... >    __second_args )
			: _Base1( __pc, core::move( __first_args ), typename __make_tuple_indices< sizeof...( _Args1 ) >::type() )
			, _Base2( __pc, core::move( __second_args ), typename __make_tuple_indices< sizeof...( _Args2 ) >::type() ) {}

	LLVM_MSTL_CONSTEXPR auto first() LLVM_MSTL_NOEXCEPT->typename _Base1::reference {
		return static_cast< _Base1& >( *this ).__get();
	}

	LLVM_MSTL_CONSTEXPR auto first() const LLVM_MSTL_NOEXCEPT->typename _Base1::const_reference {
		return static_cast< const _Base1& >( *this ).__get();
	}

	LLVM_MSTL_CONSTEXPR auto second() LLVM_MSTL_NOEXCEPT->typename _Base2::reference {
		return static_cast< _Base2& >( *this ).__get();
	}

	LLVM_MSTL_CONSTEXPR auto second() const LLVM_MSTL_NOEXCEPT->typename _Base2::const_reference {
		return static_cast< const _Base2& >( *this ).__get();
	}

	LLVM_MSTL_CONSTEXPR static auto __get_first_base( __compressed_pair* __pair ) LLVM_MSTL_NOEXCEPT->_Base1* {
		return static_cast< _Base1* >( __pair );
	}

	LLVM_MSTL_CONSTEXPR static auto __get_second_base( __compressed_pair* __pair ) LLVM_MSTL_NOEXCEPT->_Base2* {
		return static_cast< _Base2* >( __pair );
	}

	LLVM_MSTL_CONSTEXPR auto swap( __compressed_pair& __x )
		LLVM_MSTL_NOEXCEPT_V(
			core::is_nothrow_swappable_v< _T1 >&& core::is_nothrow_swappable_v< _T2 > ) {
		core::swap( first(), __x.first() );
		core::swap( second(), __x.second() );
	}

	template < typename _Tt1, typename _Tt2 >
	LLVM_MSTL_TEMPLATE_INLINE
		LLVM_MSTL_CONSTEXPR auto
		swap( __compressed_pair< _Tt1, _Tt2 >& __x, __compressed_pair< _Tt1, _Tt2 >& __y )
			LLVM_MSTL_NOEXCEPT_V(
				core::is_nothrow_swappable_v< _Tt1 >&& core::is_nothrow_swappable_v< _Tt2 > ) {
		__x.swap( __y );
	}
};

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_COMPRESS_PAIR_H
