//
// Created by dezi on 5/19/2024.
//

#ifndef LLVM_MSTL_WRAP_ITER_H
#define LLVM_MSTL_WRAP_ITER_H

#include "__config.h"

LLVM_MSTL_BEGIN_NAMESPACE_STD

template < typename _Iter >
class __wrap_iter {
public:
	using iterator_type = _Iter;
};

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_WRAP_ITER_H
