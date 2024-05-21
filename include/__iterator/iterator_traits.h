#ifndef LLVM_MSTL_ITERATOR_TRAITS_H
#define LLVM_MSTL_ITERATOR_TRAITS_H

#include "__config.h"

#include <iterator>
#include <type_traits>

LLVM_MSTL_BEGIN_NAMESPACE_STD

LLVM_MSTL_CORE_STD

/**
 * @brief Determines whether the given type has an iterator_category member type.
 * 
 * This struct is used to check if a given type `_Tp` has an `iterator_category` member type.
 * It provides a static member variable `value` that indicates whether the type has the member type.
 * 
 * @tparam _Tp The type to be checked for the presence of `iterator_category` member type.
 */
template < typename _Tp >
struct __has_iterator_category {
private:
	/**
    * @brief Helper function for testing whether the type has `iterator_category` member type.
    * @tparam _Up The type being tested.
    * @return core::false_type Always returns `core::false_type`.
    */
	template < typename _Up >
	static core::false_type __test( ... );
	/**
    * @brief Helper function for testing whether the type has `iterator_category` member type.
    * @tparam _Up The type being tested.
    * @param Pointer to `_Up::iterator_category` (default = nullptr).
    * @return core::true_type If `_Up` has `iterator_category` member type, returns `core::true_type`.
    */
	template < typename _Up >
	static core::true_type __test( typename _Up::iterator_category* = nullptr );

public:
	/**
    * @brief Indicates whether the given type has `iterator_category` member type.
    * 
    * The value is determined by invoking the `__test` function with `_Tp` as the template argument.
    * If the second overload of `__test` is selected, `value` will be `true`, indicating the presence of `iterator_category`.
    * If the first overload of `__test` is selected, `value` will be `false`, indicating the absence of `iterator_category`.
    */
	static const bool value = decltype( __test< _Tp >( nullptr ) )::value;
};

/**
 * @brief Determines if the iterator category of a type is convertible to another type.
 * 
 * This struct is used to check if the iterator category of a given type `_Tp` is convertible to another type `_Up`.
 * It provides a static member variable `value` that indicates whether the iterator category is convertible.
 * 
 * @tparam _Tp The type for which the iterator category is checked.
 * @tparam _Up The target type to which the iterator category should be convertible.
 * @tparam If the iterator category of `_Tp` is convertible to `_Up`, this parameter is `true`. Otherwise, it is `false`.
 */
template < typename _Tp, typename _Up, bool = __has_iterator_category< core::iterator_traits< _Tp > >::value >
struct __has_iterator_category_convertible_to
		: core::is_convertible< typename core::iterator_traits< _Tp >::iterator_category, _Up > {};

/**
 * @brief Specialization for when the iterator category is not convertible to the target type.
 * 
 * This specialization is used when the iterator category of the given type `_Tp` is not convertible to the target type `_Up`.
 * It inherits from `core::false_type`, indicating that the iterator category is not convertible.
 * 
 * @tparam _Tp The type for which the iterator category is checked.
 * @tparam _Up The target type to which the iterator category should be convertible.
 */
template < typename _Tp, typename _Up >
struct __has_iterator_category_convertible_to< _Tp, _Up, false > : core::false_type {};

/**
 * @brief Determines if a type is exactly a C++17 input iterator.
 * 
 * This struct is used to check if a given type `_Tp` exactly matches the requirements of a C++17 input iterator.
 * It provides a static member variable `value` that indicates whether the type is an input iterator.
 * 
 * @tparam _Tp The type to be checked for C++17 input iterator requirements.
 */
template < typename _Tp >
struct __is_exactly_cpp17_input_iterator
		: public core::integral_constant<
				bool,
				__has_iterator_category_convertible_to< _Tp, core::input_iterator_tag >::value &&
					!__has_iterator_category_convertible_to< _Tp, core::forward_iterator_tag >::value > {};


LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_ITERATOR_TRAITS_H