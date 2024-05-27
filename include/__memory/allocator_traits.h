#ifndef LLVM_MSTL_ALLOCATOR_TRAITS_H
#define LLVM_MSTL_ALLOCATOR_TRAITS_H

#include "__config.h"

#include <type_traits>

LLVM_MSTL_BEGIN_NAMESPACE_STD
LLVM_MSTL_CORE_STD

// TODO

template < typename, typename _Alloc, typename... _Args >
struct __has_construct_impl : core::false_type {};

template < typename _Alloc, typename... _Args >
struct __has_construct_impl<
	decltype( (void) core::declval< _Alloc >().construct( core::declval< _Args >()... ) ),
	_Alloc,
	_Args... > : core::true_type {};

template < typename _Alloc, typename... _Args >
struct __has_construct : __has_construct_impl< void, _Alloc, _Args... > {};

template < typename _Alloc, typename = void >
struct __is_cpp17_move_insertable
		: core::is_move_constructible< typename _Alloc::value_type > {};

template < typename _Alloc >
struct __is_cpp17_move_insertable<
	_Alloc,
	core::enable_if_t<
		!core::is_default_constructible_v< _Alloc > &&
		__has_construct<
			_Alloc,
			typename _Alloc::value_type*,
			typename _Alloc::value_type&& >::value > > : core::true_type {};

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_ALLOCATOR_TRAITS_H
