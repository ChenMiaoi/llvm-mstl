#ifndef LLVM_MSTL_ALLOCATE_AT_LEAST_H
#define LLVM_MSTL_ALLOCATE_AT_LEAST_H

#include "__config.h"

#include <memory>

LLVM_MSTL_BEGIN_NAMESPACE_STD
LLVM_MSTL_CORE_STD

/**
 * @brief Structure to hold the result of an allocation operation.
 *
 * This structure holds a pointer and a count representing the result of an allocation operation.
 *
 * @tparam _Pointer The type of the pointer.
 */
template < typename _Pointer >
struct __allocation_result {
	_Pointer ptr;  //<--- Pointer to the allocated memory
	size_t   count;//<--- Count of the allocated memory
};

/**
 * @brief Function to allocate at least a specified amount of memory using the given allocator.
 *
 * This function allocates at least the specified amount of memory using the provided allocator.
 *
 * @tparam _Alloc The allocator type.
 * @param __alloc The allocator object.
 * @param __n The number of elements to allocate.
 * @return The __allocation_result structure containing the allocated pointer and count.
 */
template < typename _Alloc >
LLVM_MSTL_NODISCARD LLVM_MSTL_CONSTEXPR auto __allocate_at_least( _Alloc& __alloc, size_t __n )
  -> __allocation_result< typename core::allocator_traits< _Alloc >::pointer > {
	return { __alloc.allocate( __n ), __n };
}

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_ALLOCATE_AT_LEAST_H