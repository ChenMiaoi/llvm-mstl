//
// Created by dezi on 5/27/2024.
//

#ifndef LLVM_MSTL_NOEXCEPT_MOVE_ASSIGN_CONTAINER_H
#define LLVM_MSTL_NOEXCEPT_MOVE_ASSIGN_CONTAINER_H

#include "__config.h"
#include <memory>
#include <type_traits>

LLVM_MSTL_BEGIN_NAMESPACE_STD
LLVM_MSTL_CORE_STD

template < typename _Alloc, typename _Traits = core::allocator_traits< _Alloc > >
struct __noexcept_move_assign_container
		: public core::integral_constant<
				bool,
				_Traits::propagate_on_container_move_assignment::value > {};

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_NOEXCEPT_MOVE_ASSIGN_CONTAINER_H
