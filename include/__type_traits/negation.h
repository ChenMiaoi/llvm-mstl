#ifndef LLVM_MSTL_NEGATION_H
#define LLVM_MSTL_NEGATION_H

#include "__config.h"
#include <type_traits>

LLVM_MSTL_BEGIN_NAMESPACE_STD
LLVM_MSTL_CORE_STD

// TODO

template < typename _Pred >
struct _Not : core::bool_constant< !_Pred::value > {};

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_NEGATION_H
