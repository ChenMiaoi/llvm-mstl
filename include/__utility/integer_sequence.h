#ifndef LLVM_MSTL_INTEGER_SEQUENCE_H
#define LLVM_MSTL_INTEGER_SEQUENCE_H

#include "__config.h"

LLVM_MSTL_BEGIN_NAMESPACE_STD

template < size_t... >
struct __tuple_indices;

template < typename _IdxType, _IdxType... _Values >
struct __integer_sequence {
	template < template < typename _OIdxType, _OIdxType... > typename _ToIndexSeq, typename _ToIndexType >
	using __convert = _ToIndexSeq< _ToIndexType, _Values... >;

	template < size_t _Sp >
	using __to_tuple_indices = __tuple_indices< ( _Values + _Sp )... >;
};

#if __has_builtin( __make_integer_seq )
template < size_t _Ep, size_t _Sp >
using __make_indices_imp =
	typename __make_integer_seq< __integer_sequence, size_t, _Ep - _Sp >::template __to_tuple_indices< _Sp >;
#endif

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_INTEGER_SEQUENCE_H