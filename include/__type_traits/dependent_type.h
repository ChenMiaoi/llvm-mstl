#ifndef LLVM_MSTL_DEPENDENT_TYPE_H
#define LLVM_MSTL_DEPENDENT_TYPE_H

#include "__config.h"

LLVM_MSTL_BEGIN_NAMESPACE_STD

template < typename _Tp, bool >
struct LLVM_MSTL_TEMPLATE_VIS __dependent_type : public _Tp {};

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_DEPENDENT_TYPE_H