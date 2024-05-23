#ifndef LLVM_MSTL_SWAP_ALLOCATOR_H
#define LLVM_MSTL_SWAP_ALLOCATOR_H

#include "__config.h"
#include <memory>
#include <type_traits>

LLVM_MSTL_BEGIN_NAMESPACE_STD
LLVM_MSTL_CORE_STD

/**
 * @brief Swaps the allocators of two objects when propagate_on_container_swap trait is true.
 * @tparam _Alloc The allocator type.
 * @param __a1 The first allocator object.
 * @param __a2 The second allocator object.
 *
 * This function swaps the allocators of two objects, __a1 and __a2,
 * when the propagate_on_container_swap trait of the allocator type is true.
 * The allocators are swapped using core::swap.
 * The function is noexcept if the allocator type is nothrow swappable.
 */
template < typename _Alloc >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __swap_allocator( _Alloc& __a1, _Alloc& __a2, core::true_type )
	LLVM_MSTL_NOEXCEPT_V( core::is_nothrow_swappable_v< _Alloc > ) {
	core::swap( __a1, __a2 );
}

/**
 * @brief Swaps the allocators of two objects when propagate_on_container_swap trait is false.
 * @tparam _Alloc The allocator type.
 * @param __a1 The first allocator object.
 * @param __a2 The second allocator object.
 *
 * This function performs no operation as the propagate_on_container_swap trait of the allocator type is false.
 * The function is noexcept.
 */
template < typename _Alloc >
LLVM_MSTL_TEMPLATE_INLINE
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
	__swap_allocator( _Alloc& __a1, _Alloc& __a2, core::false_type ) LLVM_MSTL_NOEXCEPT {}

/**
 * @brief Swaps the allocators of two objects conditionally based on propagate_on_container_swap trait.
 * @tparam _Alloc The allocator type.
 * @param __a1 The first allocator object.
 * @param __a2 The second allocator object.
 *
 * This function swaps the allocators of two objects, __a1 and __a2, based on the propagate_on_container_swap trait.
 * If the propagate_on_container_swap trait of the allocator type is true, the allocators are swapped using core::swap.
 * If the propagate_on_container_swap trait is false, no operation is performed.
 * The function uses type traits to determine the value of propagate_on_container_swap and whether the allocator is nothrow swappable.
 */
template < typename _Alloc >
LLVM_MSTL_TEMPLATE_INLINE LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __swap_allocator( _Alloc& __a1, _Alloc& __a2 )
	LLVM_MSTL_NOEXCEPT_V( core::is_nothrow_swappable_v< _Alloc > ) {
	__swap_allocator(
		__a1,
		__a2,
		core::integral_constant< bool, core::allocator_traits< _Alloc >::propagate_on_container_swap::value >() );
}

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_SWAP_ALLOCATOR_H