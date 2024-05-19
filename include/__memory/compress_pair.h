//
// Created by dezi on 5/19/2024.
//

#ifndef LLVM_MSTL_COMPRESS_PAIR_H
#define LLVM_MSTL_COMPRESS_PAIR_H

#include "__config.h"

#include <type_traits>

LLVM_MSTL_BEGIN_NAMESPACE_STD

LLVM_MSTL_CORE_STD

struct __default_init_tag {};
struct __value_init_tag {};

template < typename _Tp, int32_t _Idx, bool _CanBeEmptyBase = core::is_empty_v< _Tp >&& __cpp_lib_is_final >
struct __compressed_pair_elem {
};

template < typename _Tp, int32_t _Idx >
struct __compressed_pair_elem< _Tp, _Idx, true > : private _Tp {};

template < typename _T1, typename _T2 >
class __compressed_pair : private __compressed_pair_elem< _T1, 0 >,
                          private __compressed_pair_elem< _T2, 1 > {
};

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_COMPRESS_PAIR_H
