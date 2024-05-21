#ifndef LLVM_MSTL_IS_ALLOCATOR_H
#define LLVM_MSTL_IS_ALLOCATOR_H

#include "__config.h"
#include "__type_traits/void_t.h"
#include <cstddef>
#include <type_traits>

LLVM_MSTL_BEGIN_NAMESPACE_STD

LLVM_MSTL_CORE_STD

/**
 * @brief Type trait to check if a type satisfies the requirements of an Allocator.
 *
 * This type trait is used to check if a given type `_Alloc` satisfies the requirements of an Allocator.
 * It is a metafunction that provides a static constant boolean value `value` which is true if `_Alloc` is an Allocator,
 * and false otherwise.
 *
 * The trait checks the presence of the following two conditions:
 * 1. `_Alloc` must have a nested type `value_type`.
 * 2. `_Alloc` must have a member function `allocate` that takes a `size_t` argument and returns a pointer.
 *
 * If both conditions are satisfied, the trait's `value` is set to true; otherwise, it is set to false.
 *
 * @tparam _Alloc The type to be checked if it is an Allocator.
 * @tparam void Additional template parameter used for SFINAE pattern.
 * @tparam void Additional template parameter used for SFINAE pattern.
 */
template < typename _Alloc, typename = void, typename = void >
struct __is_allocator : core::false_type {};


/**
 * @brief Specialization of `__is_allocator` for types that satisfy the Allocator requirements.
 *
 * This specialization is used when the type `_Alloc` satisfies the requirements of an Allocator.
 * It sets the static constant boolean value `value` to true.
 *
 * The specialization checks the presence of the following two conditions:
 * 1. `_Alloc` must have a nested type `value_type`.
 * 2. `_Alloc` must have a member function `allocate` that takes a `size_t` argument and returns a pointer.
 *
 * If both conditions are satisfied, the specialization's `value` is set to true; otherwise, it defaults to false.
 *
 * @tparam _Alloc The type to be checked if it is an Allocator.
 */
template < typename _Alloc >
struct __is_allocator<
  _Alloc,
  __void_t< typename _Alloc::value_type >,
  __void_t< decltype( core::declval< _Alloc& >().allocate( core::size_t( 0 ) ) ) > > : core::true_type {
};

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_IS_ALLOCATOR_H