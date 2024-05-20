#ifndef LLVM_MSTL_IS_ALLOCATOR_H
#define LLVM_MSTL_IS_ALLOCATOR_H

#include "__config.h"
#include "__type_traits/void_t.h"
#include <cstddef>
#include <type_traits>

LLVM_MSTL_BEGIN_NAMESPACE_STD

LLVM_MSTL_CORE_STD

template < typename _Alloc, typename = void, typename = void >
struct __is_allocator : core::false_type {};

template < typename _Alloc >
struct __is_allocator<
  _Alloc,
  __void_t< typename _Alloc::value_type >,
  __void_t< decltype( core::declval< _Alloc& >().allocate( core::size_t( 0 ) ) ) > > : core::true_type {
};
LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_IS_ALLOCATOR_H