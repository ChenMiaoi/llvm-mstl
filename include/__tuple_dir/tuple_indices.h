#ifndef LLVM_MSTL_TUPLE_INDICES_H
#define LLVM_MSTL_TUPLE_INDICES_H

#include "__config.h"
#include "__utility/integer_sequence.h"

LLVM_MSTL_BEGIN_NAMESPACE_STD

template < size_t... >
struct __tuple_indices {};

/**
 * @brief make tuple integer sequence generate
 * 
 * @tparam _Ep end offset
 * @tparam _Sp start offset
 *
 * @code{.cc}
 * 
 * // if we specific the __tuple_indices
 * template < size_t ... Indices>
 * struct __tuple_indices {
 *   static void print() {
 *     core::cout << "indices: ";
 *     ((core::cout << Indices << ' '), ...);
 *     core::cout << "\n";
 *   }
 * };
 * 
 * // generate integer sequence [2, 3, 4]
 * typename __make_tuple_indices<5, 2>::type::print(); 
 *
 * @endcode
 * 
 */
template < size_t _Ep, size_t _Sp = 0 >
struct __make_tuple_indices {
	static_assert( _Sp <= _Ep, "__make_tuple_indices input error" );
	using type = __make_indices_imp< _Ep, _Sp >;
};

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_TUPLE_INDICES_H