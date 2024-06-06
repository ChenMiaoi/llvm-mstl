#ifndef LLVM_MSTL_UNINITIALIZED_ALGORITHMS_H
#define LLVM_MSTL_UNINITIALIZED_ALGORITHMS_H

#include "__config.h"
#include "__memory/allocator_traits.h"
#include "__type_traits/negation.h"
#include "__utility/exception_guard.h"

#include <iterator>
#include <memory>
#include <type_traits>

LLVM_MSTL_BEGIN_NAMESPACE_STD
LLVM_MSTL_CORE_STD

/**
 * @brief Destroy objects in the allocated memory block.
 *
 * This function is used to destroy (deallocate) objects in the memory block pointed to by `_alloc`,
 * from the iterator `_first` to `_last`.
 *
 * @tparam _Alloc The allocator type.
 * @tparam _Iter The iterator type.
 * @tparam _Send The sentinel type.
 * @param __alloc The allocator reference.
 * @param __first The iterator pointing to the beginning of the object sequence to be destroyed.
 * @param __last The sentinel iterator pointing to the end of the object sequence to be destroyed.
 *
 * @return None.
 */
template < typename _Alloc, typename _Iter, typename _Send >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __alloctor_destroy( _Alloc& __alloc, _Iter __first, _Send __last ) {
	for ( ; __first != __last; ++__first ) {
		core::allocator_traits< _Alloc >::destroy( __alloc, core::to_address( __first ) );
	}
}

/**
 * @brief Functor for destroying objects in reverse order within an allocated memory block.
 *
 * This class is a functor used to destroy (deallocate) objects in reverse order within the memory block
 * pointed to by `_alloc`, from the iterator `_first` to `_last`.
 *
 * @tparam _Alloc The allocator type.
 * @tparam _Iter The iterator type.
 */
template < typename _Alloc, typename _Iter >
class _AllocatorDestroyRangeReverse {
public:
	/**
	* @brief Constructs a `_AllocatorDestroyRangeReverse` functor object.
	*
	* @param __alloc The allocator reference.
	* @param __first The iterator reference pointing to the beginning of the object sequence to be destroyed.
	* @param __last The iterator reference pointing to the end of the object sequence to be destroyed.
	*/
	LLVM_MSTL_CONSTEXPR _AllocatorDestroyRangeReverse( _Alloc& __alloc, _Iter& __first, _Iter& __last )
			: __alloc( __alloc )
			, __first( __first )
			, __last( __last ) {}

	/**
	* @brief Function call operator to destroy objects in reverse order.
	*
	* This operator is called to destroy objects in reverse order within the memory block.
	*/
	LLVM_MSTL_CONSTEXPR void operator()() const {
		__alloctor_destroy(
			__alloc,
			core::reverse_iterator< _Iter >( __last ),
			core::reverse_iterator< _Iter >( __first ) );
	}

private:
	_Alloc& __alloc;//<--- The allocator reference
	_Iter&  __first;//<--- The iterator reference pointing to the beginning of the object sequence.
	_Iter&  __last; //<--- The iterator reference pointing to the end of the object sequence.
};

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
 * @brief Copy-construct objects using an allocator and uninitialized memory.
 *
 * This function is used to copy-construct objects from the range `[__first1, __last1)` using the allocator
 * and uninitialized memory starting at the address pointed to by `__first2`.
 *
 * @tparam _Alloc The allocator type.
 * @tparam _Iter1 The input iterator type for the source range.
 * @tparam _Sent1 The sentinel type for the source range.
 * @tparam _Iter2 The output iterator type for the destination range.
 * @param __alloc The allocator reference.
 * @param __first1 The input iterator pointing to the beginning of the source range.
 * @param __last1 The sentinel iterator pointing to the end of the source range.
 * @param __first2 The output iterator pointing to the beginning of the destination range.
 * @return The output iterator pointing to the end of the constructed objects in the destination range.
 */
template < typename _Alloc, typename _Iter1, typename _Sent1, typename _Iter2 >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __uninitialized_allocator_copy(
	_Alloc& __alloc, _Iter1 __first1, _Sent1 __last1, _Iter2 __first2 ) -> _Iter2 {
	auto __destruct_first = __first2;
	auto __guard          = __make_exception_guard(
    _AllocatorDestroyRangeReverse< _Alloc, _Iter2 >( __alloc, __destruct_first, __first2 ) );
	//!<--- When an exception occurs in the logical code below that
	//!<--- causes the code to exit directly (without passing through '__guard.__complete()'),
	//!<--- then the destruction procedure is executed, so that the resources
	//!<--- in the requested [__last, __first2) interval can be released normally

	//<--- This is the range of Iterator: [__first1, __last1)
	//<--- We want to do that use the `__first1` to initialize the allocate address start at `__first2`'s address
	while ( __first1 != __last1 ) {
		core::allocator_traits< _Alloc >::construct( __alloc, core::to_address( __first2 ), *__first1 );
		++__first1;
		++__first2;
	}
	__guard.__complete();
	return __first2;
}

template <
	typename _Alloc,
	typename _Type,
	typename _RawType = core::remove_const_t< _Type >,
	core::enable_if_t<
		core::is_trivially_copy_constructible_v< _RawType > &&
		core ::is_trivially_copy_assignable_v< _RawType > &&
		__allocator_has_trivial_copy_construct< _Alloc, _RawType >::value >* = nullptr >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __uninitialized_allocator_copy(
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


/**
 * @brief Move-construct objects using an allocator and uninitialized memory if noexcept.
 *
 * This function is used to move-construct objects from the range `[__first1, __last1)` using the allocator
 * and uninitialized memory starting at the address pointed to by `__first2`. It only performs move construction
 * if the move constructor of the object type is declared as `noexcept`.
 *
 * @tparam _Alloc The allocator type.
 * @tparam _Iter1 The input iterator type for the source range.
 * @tparam _Sent1 The sentinel type for the source range.
 * @tparam _Iter2 The output iterator type for the destination range.
 * @param __alloc The allocator reference.
 * @param __first1 The input iterator pointing to the beginning of the source range.
 * @param __last1 The sentinel iterator pointing to the end of the source range.
 * @param __first2 The output iterator pointing to the beginning of the destination range.
 * @return The output iterator pointing to the end of the constructed objects in the destination range.
 *
 * @remark The function first checks if the specified type meets the requirements of `Cpp17MoveInsertable`. 
 * If not, a static assertion fails.
 *
 * The function then creates a guard object (`__guard`) to ensure proper destruction of allocated objects in case of an exception. 
 * It uses the `_AllocatorDestroyRangeReverse` helper class to destroy the objects in reverse order.
 *
 * The function iterates over the source range `[__first1, __last1)` and uses the allocator to 
 * move-construct objects at the uninitialized memory locations starting from `__first2`. 
 * It calls `core::allocator_traits<_Alloc>::construct()` to construct the objects by invoking their move constructor 
 * if it is declared as `noexcept`. It increments both `__first1` and `__first2` after each construction.
 *
 * Finally, the `__guard.__complete()` function is called to indicate that the construction phase is complete. 
 * If an exception occurs during construction and the code exits directly without passing through `__guard.__complete()`, 
 * the destruction procedure will be executed to release the resources in the range `[__last, __first2)`.
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
	//!<--- When an exception occurs in the logical code below that
	//!<--- causes the code to exit directly (without passing through '__guard.__complete()'),
	//!<--- then the destruction procedure is executed, so that the resources
	//!<--- in the requested [__last, __first2) interval can be released normally

	while ( __first1 != __last1 ) {
		core::allocator_traits< _Alloc >::construct( __alloc, core::to_address( __first2 ), core::move_if_noexcept( *__first1 ) );
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
