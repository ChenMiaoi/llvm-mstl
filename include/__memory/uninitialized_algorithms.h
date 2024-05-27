#ifndef LLVM_MSTL_UNINITIALIZED_ALGORITHMS_H
#define LLVM_MSTL_UNINITIALIZED_ALGORITHMS_H

#include "__config.h"
#include "__memory/allocator_traits.h"
#include "__type_traits/negation.h"
#include "__utility/exception_guard.h"

#include <iterator>
#include <memory>
#include <type_traits>
#include <xutility>

LLVM_MSTL_BEGIN_NAMESPACE_STD
LLVM_MSTL_CORE_STD

/**
 * @brief TODO
 * 
 * @tparam _Alloc 
 * @tparam _Iter 
 * @tparam _Send 
 * @param __alloc 
 * @param __first 
 * @param __last 
 * @return LLVM_MSTL_CONSTEXPR_SINCE_CXX20 
 */
template < typename _Alloc, typename _Iter, typename _Send >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __alloctor_destroy( _Alloc& __alloc, _Iter __first, _Send __last ) {
	for ( ; __first != __last; ++__first ) {
		core::allocator_traits< _Alloc >::destroy( __alloc, core::to_address( __first ) );
	}
}

/**
 * @brief TODO
 * 
 * @tparam _Alloc 
 * @tparam _Iter 
 */
template < typename _Alloc, typename _Iter >
class _AllocatorDestroyRangeReverse {
public:
	LLVM_MSTL_CONSTEXPR _AllocatorDestroyRangeReverse( _Alloc& __alloc, _Iter& __first, _Iter& __last )
			: __alloc( __alloc )
			, __first( __first )
			, __last( __last ) {}

	LLVM_MSTL_CONSTEXPR void operator()() const {
		__alloctor_destroy(
			__alloc,
			core::reverse_iterator< _Iter >( __last ),
			core::reverse_iterator< _Iter >( __first ) );
	}

private:
	_Alloc& __alloc;
	_Iter&  __first;
	_Iter&  __last;
};

/**
 * @brief TODO
 * 
 * @tparam _Alloc 
 * @tparam _Iter1 
 * @tparam _Send1 
 * @tparam _Iter2 
 * @param __alloc 
 * @param __first1 
 * @param __last1 
 * @param __first2 
 * @return _Iter2 
 */
template < typename _Alloc, typename _Iter1, typename _Send1, typename _Iter2 >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __uninitialized_allocator_move_if_noexcept(
	_Alloc& __alloc, _Iter1 __first1, _Send1 __last1, _Iter2 __first2 ) -> _Iter2 {
	static_assert(
		__is_cpp17_move_insertable< _Alloc >::value,
		"The specified type does not meet the requirements of Cpp17MoveInsertable" );

	auto __destruct_first = __first2;
	auto __guard          = __make_exception_guard(
    _AllocatorDestroyRangeReverse< _Alloc, _Iter2 >( __alloc, __destruct_first, __first2 ) );

	while ( __first1 != __last1 ) {
		core::allocator_traits< _Alloc >::construct( __alloc, core::to_address( __first2 ), *__first1 );
		++__first1;
		++__first2;
	}
	__guard.__complete();
	return __first2;
}

/**
 * @brief TODO
 * 
 * @tparam _Alloc 
 * @tparam _Type 
 */
template < typename _Alloc, typename _Type >
struct __allocator_has_trivial_copy_construct
		: _Not< __has_construct< _Alloc, _Type*, const _Type& > > {};

template < typename _Type >
struct __allocator_has_trivial_copy_construct< core::allocator< _Type >, _Type >
		: core::true_type {};

/**
 * @brief TODO
 * 
 * @tparam _Alloc 
 * @tparam _Type 
 * @tparam core::remove_const_t< _Type > 
 * @param __first1 
 * @param __last1 
 * @param __first2 
 * @return _Type* 
 */
template <
	typename _Alloc,
	typename _Type,
	typename _RawType = core::remove_const_t< _Type >,
	core::enable_if_t<
		core::is_trivially_copy_constructible_v< _RawType > &&
		core::is_trivially_copy_assignable_v< _RawType > &&
		__allocator_has_trivial_copy_construct< _Alloc, _RawType >::value >* = nullptr >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
__uninitialized_allocator_move_if_noexcept(
	_Alloc&, const _Type* __first1, const _Type* __last1, _Type* __first2 ) -> _Type* {
	if ( core::is_constant_evaluated() ) {
		while ( __first1 != __last1 ) {
			core::construct_at( core::to_address( __first2 ), *__first1 );
			++__first1;
			++__first2;
		}
		return __first2;
	} else {
		return core::copy( __first1, __last1, const_cast< _RawType* >( __first2 ) );
	}
}

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_UNINITIALIZED_ALGORITHMS_H
