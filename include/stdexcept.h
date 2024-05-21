//
// Created by dezi on 5/20/2024.
//

#ifndef LLVM_MSTL_STDEXCEPT_H
#define LLVM_MSTL_STDEXCEPT_H

#include "__config.h"

#include <cstdlib>

LLVM_MSTL_BEGIN_NAMESPACE_STD

LLVM_MSTL_CORE_STD

LLVM_MSTL_NORETURN LLVM_MSTL_INLINE void __throw_length_error( const char* __msg ) {
	static_cast< void >( __msg );
	core::abort();
}

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_STDEXCEPT_H
