#ifndef LLVM_MSTL_VECTOR_H
#define LLVM_MSTL_VECTOR_H

/**
 * @file vector.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-05-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "__config.h"
#include "__iterator/iterator_traits.h"
#include "__iterator/wrap_iter.h"
#include "__memory/allocate_at_least.h"
#include "__memory/compress_pair.h"
#include "__memory/uninitialized_algorithms.h"
#include "__split_buffer.h"
#include "__type_traits/is_allocator.h"
#include "__type_traits/noexcept_move_assign_container.h"
#include "__utility/exception_guard.h"
// #include "__utility/logger.h"
#include "stdexcept.h"

#include "spdlog/spdlog.h"


#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <stdio.h>
#include <type_traits>


LLVM_MSTL_BEGIN_NAMESPACE_STD
LLVM_MSTL_CORE_STD

/**
 * @brief 
 * 
 * @tparam _Tp 
 * @tparam _Allocator 
 *
 * @code
 * 	template < class T, class Allocator = allocator<T> >
 * 	class vector {
 *	public:
 *		typedef T                                        value_type;
 *		typedef Allocator                                allocator_type;
 *		typedef typename allocator_type::reference       reference;
 * 		typedef typename allocator_type::const_reference const_reference;
 *		typedef implementation-defined                   iterator;
 *		typedef implementation-defined                   const_iterator;
 *		typedef typename allocator_type::size_type       size_type;
 *		typedef typename allocator_type::difference_type difference_type;
 *		typedef typename allocator_type::pointer         pointer;
 *		typedef typename allocator_type::const_pointer   const_pointer;
 *		typedef std::reverse_iterator<iterator>          reverse_iterator;
 *		typedef std::reverse_iterator<const_iterator>    const_reverse_iterator;
 *
 *		vector()
 *			noexcept(is_nothrow_default_constructible<allocator_type>::value);
 *		explicit vector(const allocator_type&);
 *		explicit vector(size_type n);
 *		explicit vector(size_type n, const allocator_type&); // C++14
 *		vector(size_type n, const value_type& value, const allocator_type& = allocator_type());
 *		template <class InputIterator>
 *		vector(InputIterator first, InputIterator last, const allocator_type& = allocator_type());
 *		vector(const vector& x);
 *		vector(vector&& x)
 *			noexcept(is_nothrow_move_constructible<allocator_type>::value);
 *		vector(initializer_list<value_type> il);
 *		vector(initializer_list<value_type> il, const allocator_type& a);
 *		~vector();
 *		vector& operator=(const vector& x);
 *		vector& operator=(vector&& x) noexcept(
 *			allocator_type::propagate_on_container_move_assignment::value ||
 *			allocator_type::is_always_equal::value); // C++17
 *		vector& operator=(initializer_list<value_type> il);
 *		template <class InputIterator>
 *		void assign(InputIterator first, InputIterator last);
 *		void assign(size_type n, const value_type& u);
			void assign(initializer_list<value_type> il);

			allocator_type get_allocator() const noexcept;

			iterator               begin() noexcept;
			const_iterator         begin()   const noexcept;
			iterator               end() noexcept;
			const_iterator         end()     const noexcept;

			reverse_iterator       rbegin() noexcept;
			const_reverse_iterator rbegin()  const noexcept;
			reverse_iterator       rend() noexcept;
			const_reverse_iterator rend()    const noexcept;

			const_iterator         cbegin()  const noexcept;
				const_iterator         cend()    const noexcept;
				const_reverse_iterator crbegin() const noexcept;
				const_reverse_iterator crend()   const noexcept;

				size_type size() const noexcept;
				size_type max_size() const noexcept;
				size_type capacity() const noexcept;
				bool empty() const noexcept;
				void reserve(size_type n);
				void shrink_to_fit() noexcept;

				reference       operator[](size_type n);
				const_reference operator[](size_type n) const;
				reference       at(size_type n);
				const_reference at(size_type n) const;

				reference       front();
				const_reference front() const;
				reference       back();
				const_reference back() const;

				value_type*       data() noexcept;
				const value_type* data() const noexcept;

				void push_back(const value_type& x);
				void push_back(value_type&& x);
				template <class... Args>
						reference emplace_back(Args&&... args); // reference in C++17
				void pop_back();

				template <class... Args> iterator emplace(const_iterator position, Args&&... args);
				iterator insert(const_iterator position, const value_type& x);
				iterator insert(const_iterator position, value_type&& x);
				iterator insert(const_iterator position, size_type n, const value_type& x);
				template <class InputIterator>
						iterator insert(const_iterator position, InputIterator first, InputIterator last);
				iterator insert(const_iterator position, initializer_list<value_type> il);

				iterator erase(const_iterator position);
				iterator erase(const_iterator first, const_iterator last);

				void clear() noexcept;

				void resize(size_type sz);
				void resize(size_type sz, const value_type& c);

				void swap(vector&)
						noexcept(allocator_traits<allocator_type>::propagate_on_container_swap::value ||
										allocator_traits<allocator_type>::is_always_equal::value);  // C++17

				bool __invariants() const;
		};
 * @endcode
 * 
 */
template < typename _Tp, typename _Allocator >
class LLVM_MSTL_TEMPLATE_VIS vector {
private:
	using __default_allocator_type = core::allocator< _Tp >;

public:
	/**
	 * @ref vector member types https://en.cppreference.com/w/cpp/container/vector
	 */
	using __self          = vector;
	using value_type      = _Tp;
	using allocator_type  = _Allocator;
	using __alloc_traits  = core::allocator_traits< allocator_type >;
	using reference       = value_type&;
	using const_reference = const value_type&;
	using size_type       = typename __alloc_traits::size_type;
	using difference_type = typename __alloc_traits::difference_type;
	using pointer         = typename __alloc_traits::pointer;
	using const_pointer   = typename __alloc_traits::const_pointer;

	using iterator               = __wrap_iter< pointer >;
	using const_iterator         = __wrap_iter< const_pointer >;
	using reverse_iterator       = core::reverse_iterator< iterator >;
	using const_reverse_iterator = core::reverse_iterator< const_iterator >;

	static_assert(
		(core::is_same_v< typename allocator_type::value_type, value_type >),
		"Allocator::value_type must be same type as value_type" );

	static_assert(
		core::is_same_v< allocator_type, typename __alloc_traits::template rebind_alloc< value_type > >,
		"[allocator.requirements] states that rebinding an allocator to the same type should result in the "
		"original allocator" );

	/*************************************************************************************		
	 *                                                                                   *
	 *															CONSTRUCTOR BEGIN		                                 *
	 *                                                                                   *
	 *************************************************************************************/

	/**
		* @brief Default constructor for creating an empty vector.
		* 
		* @ref (1) Default constructor. Constructs an empty container with a default-constructed allocator.
		*  		https://en.cppreference.com/w/cpp/container/vector/vector
		*
		* This constructor creates an empty vector object without any parameters.
		* It is available since C++20 and can be evaluated as a constant expression.
		* The constructor is declared as noexcept if the allocator_type's default constructor
		* is marked as noexcept.
		*
		* @tparam allocator_type The type of the allocator used by the vector.
		*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector()
		LLVM_MSTL_NOEXCEPT_V( core::is_nothrow_default_constructible_v< allocator_type > ){};

	/**
		* @brief Explicit constructor for creating a vector with a specified allocator.
		*
		* @ref (2) Constructs an empty container with the given allocator alloc. 
		*			https://en.cppreference.com/w/cpp/container/vector/vector
		*
		* This constructor creates a vector object with a specified allocator.
		* It is available since C++20 and can be evaluated as a constant expression.
		* The constructor is declared as noexcept.
		*
		* @tparam allocator_type The type of the allocator used by the vector.
		* @param __a The allocator object to be used by the vector.
		*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 explicit vector( const allocator_type& __a ) LLVM_MSTL_NOEXCEPT
			: __end_capm( nullptr, __a ) {}

	/**
		* @brief Constructs a vector with a specified size.
		*
		* @ref (4) Constructs the container with `count` default-inserted instances of `T`. No copies are made.
		*			 https://en.cppreference.com/w/cpp/container/vector/vector
		*
		* This constructor initializes a vector with a specified size, creating `__n` elements.
		* It allocates memory using the specified allocator, constructs elements at the end of the vector,
		* and completes the construction process.
	  *
	  * @note This constructor will initializes a vector with a specified size and initialize all elements to `zero`
		*
		* @tparam _Tp The type of elements stored in the vector.
		* @tparam _Allocator The allocator type used for memory allocation.
		* @param __n The size of the vector.
		*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 explicit vector( size_type __n );

	/**
		* @brief Constructs a vector with a specified size and allocator.
		* 
		* @ref (4) Constructs the container with `count` default-inserted instances of `T`. No copies are made.
		*			 https://en.cppreference.com/w/cpp/container/vector/vector
		*
		* @param __n The initial size of the vector.
		* @param __a The allocator to use for memory allocation.
		*
		* This constructor initializes a vector with `__n` default-constructed elements
		* and uses the specified allocator (`__a`) for memory allocation.
		*
		* If `__n` is greater than zero, memory is allocated using the allocator and
		* default-constructed elements are created using placement new. The vector's
		* `__end_capm` member is initialized with a nullptr and the allocator (`__a`).
		*
		* If an exception is thrown during the construction, the vector is destroyed
		* and the exception is rethrown.
		*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 explicit vector( size_type __n, const allocator_type& __a );

	/**
		* @brief Constructs a vector with a specified size and initial value.
		*
		* @ref (3) Constructs the container with count copies of elements with value value.
		*			 https://en.cppreference.com/w/cpp/container/vector/vector
		*
		* @param __n The initial size of the vector.
		* @param __x The initial value to be assigned to each element.
		*
		* This constructor initializes a vector with `__n` copies of the specified initial value (`__x`).
		*
		* If `__n` is greater than zero, memory is allocated and default-constructed elements are created
		* using placement new. The vector's `__end_capm` member is initialized with a nullptr and the default allocator.
		*
		* If an exception is thrown during the construction, the vector is destroyed and the exception is rethrown.
		*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector( size_type __n, const value_type& __x );

	/**
	* @brief Constructs a vector with a specified number of elements, each initialized to a given value,
	*        using a specified allocator.
	*
	* @ref (3) Constructs the container with count copies of elements with value value.
	*			 https://en.cppreference.com/w/cpp/container/vector/vector
	* 
	* This constructor creates a vector with `__n` elements, each initialized to the value `__x`, using the allocator `__a`.
	* The constructor requires that the type `_Allocator` satisfies the requirements of an Allocator, as determined by
	* the `__is_allocator` type trait.
	*
	* If the allocator requirements are satisfied, the constructor allocates memory for `__n` elements using `__vallocate`,
	* and then constructs the elements at the end of the vector with the value `__x` using `__construct_at_end`.
	*
	* @tparam _Allocator The allocator type used for memory allocation.
	* @param __n The number of elements to create.
	* @param __x The value to initialize each element with.
	* @param __a The allocator instance used for memory allocation.
	*/
	template < typename = core::enable_if_t< __is_allocator< _Allocator >::value > >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	vector( size_type __n, const value_type& __x, const allocator_type& __a )
			: __end_capm( nullptr, __a ) {
		if ( __n > 0 ) {
			__vallocate( __n );
			__construct_at_end( __n, __x );
		}
	}

	/**
	* @brief Constructs a vector from a range defined by two input iterators.
	*
	* @ref (5) Constructs the container with the contents of the range [first, last).
	* 		 https://en.cppreference.com/w/cpp/container/vector/vector
	* 
	* This constructor constructs a vector by inserting elements from a range defined by two input iterators,
	* effectively copying the elements in the range to the vector.
	*
	* @code{cc}  
	* // Create a vector from a range defined by input iterators
	* int arr[] = { 1, 2, 3, 4, 5 };
	* nya::vector<int> vec( arr, arr + 5 );
	* @endcode  
	* 
	* @tparam _InputIterator The type of the input iterators defining the range.
	* @param __first The beginning of the range to construct the vector from.
	* @param __last The end of the range to construct the vector from.
	* @throws Any exception thrown during element construction or memory allocation.
	*/
	template <
		typename _InputIterator,
		core::enable_if_t<
			__is_exactly_cpp17_input_iterator< _InputIterator >::value &&
				core::is_constructible_v<
					value_type,
					typename core::iterator_traits< _InputIterator >::reference >,
			int > = 0 >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector( _InputIterator __first, _InputIterator __last );

	/**
	* @brief Constructs a vector from a range defined by two input iterators.
	*
	* @ref (5) Constructs the container with the contents of the range [first, last).
	* 		 https://en.cppreference.com/w/cpp/container/vector/vector
	* 
	* This constructor constructs a vector by inserting elements from a range defined by two input iterators,
	* effectively copying the elements in the range to the vector.
	*
	* @code{cc}  
	* // Create a vector from a range defined by input iterators
	* int arr[] = { 1, 2, 3, 4, 5 };
	* core::allocator<int> __a;
	* nya::vector<int> vec( arr, arr + 5, __a );
	* @endcode  
	* 
	* @tparam _InputIterator The type of the input iterators defining the range.
	* @param __first The beginning of the range to construct the vector from.
	* @param __last The end of the range to construct the vector from.
	* @param __a The allocator object used to allocate memory for the vector.
	* @throws Any exception thrown during element construction or memory allocation.
	*/
	template <
		typename _InputIterator,
		core::enable_if_t<
			__is_exactly_cpp17_input_iterator< _InputIterator >::value &&
				core::is_constructible_v<
					value_type,
					typename core::iterator_traits< _InputIterator >::reference >,
			int > = 0 >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector( _InputIterator __first, _InputIterator __last, const allocator_type& __a );

	/**
	* @brief Constructor that initializes the vector container with a range of iterators.
	*
	* @ref (5) Constructs the container with the contents of the range [first, last).
	* 		 https://en.cppreference.com/w/cpp/container/vector/vector
	* 
	* This constructor takes a range of forward iterators and initializes the vector container
	* with the elements in that range. The elements are copied from the range and stored in the
	* vector container.
	* 
	* @code{cc}  
	* // Create a vector from a range defined by forward iterators
	* nya::vector<int> __v({1, 2, 3, 4, 5});
	* core::allocator<int> __a;
	* nya::vector<int> vec( __v.begin(), __v.end() );
	* @endcode  
	*
	* @tparam _Tp The type of elements.
	* @tparam _Allocator The allocator type.
	* @tparam _ForwardIterator The iterator type.
	* 
	* @param __first The starting iterator of the range.
	* @param __last The ending iterator of the range.
	*/
	template <
		typename _ForwardIterator,
		core::enable_if_t<
			__is_cpp17_forward_iterator< _ForwardIterator >::value &&
				core::is_constructible_v<
					value_type,
					typename core::iterator_traits< _ForwardIterator >::reference >,
			int > = 0 >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector( _ForwardIterator __first, _ForwardIterator __last );

	/**
	* @brief Constructor that initializes the vector container with a range of iterators.
	*
	* @ref (5) Constructs the container with the contents of the range [first, last).
	* 		 https://en.cppreference.com/w/cpp/container/vector/vector
	* 
	* This constructor takes a range of forward iterators and initializes the vector container
	* with the elements in that range. The elements are copied from the range and stored in the
	* vector container.
	* 
	* @code{cc}  
	* // Create a vector from a range defined by forward iterators
	* nya::vector<int> __v({1, 2, 3, 4, 5});
	* core::allocator<int> __a;
	* nya::vector<int> vec( __v.begin(), __v.end(), __a );
	* @endcode  
	*
	* @tparam _Tp The type of elements.
	* @tparam _Allocator The allocator type.
	* @tparam _ForwardIterator The iterator type.
	* 
	* @param __first The starting iterator of the range.
	* @param __last The ending iterator of the range.
	*/
	template <
		typename _ForwardIterator,
		core::enable_if_t<
			__is_cpp17_forward_iterator< _ForwardIterator >::value &&
				core::is_constructible_v<
					value_type,
					typename core::iterator_traits< _ForwardIterator >::reference >,
			int > = 0 >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector( _ForwardIterator __first, _ForwardIterator __last, const allocator_type& __a );

	/**
	* @brief Constructs a vector by copying another vector.
	*
	* @ref (6) Copy constructor. Constructs the container with the copy of the contents of other. 
	* 		 https://en.cppreference.com/w/cpp/container/vector/vector
	*
	* This constructor initializes a vector by performing a copy of the elements from another vector `__x`. 
	* It uses the `__alloc_traits::select_on_container_copy_construction` function to select the appropriate allocator 
	* for the new vector based on `__x.__alloc()`.
	*
	* @tparam _Tp The value type of the vector.
	* @tparam _Allocator The allocator type used for memory management.
	* @param __x The vector to be copied.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector( const vector& __x );

	/**
	* @brief Constructs a vector by copying another vector with a specified allocator.
	*
	* @ref (7) Constructs the container with the copy of the contents of other, using alloc as the allocator.
	* 		 https://en.cppreference.com/w/cpp/container/vector/vector
	* 
	* This constructor initializes a vector by performing a copy of the elements from another vector `__x` 
	* while using a specified allocator `__a`. It sets the end-capacity pointer (`__end_capm`) to `nullptr` 
	* and assigns the allocator `__a` to the vector.
	*
	* @tparam _Tp The value type of the vector.
	* @tparam _Allocator The allocator type used for memory management.
	* @param __x The vector to be copied.
	* @param __a The allocator to be used for the new vector. It must be the same type as the allocator type of the vector being constructed.
	*
	* @remark The `__a` parameter must have the same type as the allocator type `_Allocator` of the vector being constructed. 
	* This requirement ensures that the allocator used for the new vector is compatible with the allocator of the copied vector. 
	* Using a different allocator type may result in undefined behavior or compilation errors.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector( const vector& __x, const core::type_identity_t< allocator_type >& __a );

	/**
	* @brief Constructs a vector from an initializer list.
	*
	* @ref (10) Constructs the container with the contents of the initializer list init. 
	*			 https://en.cppreference.com/w/cpp/container/vector/vector
	*
	* This constructor initializes a vector with the elements from the specified initializer list `__il`. 
	* It sets the end-capacity pointer (`__end_capm`) to `nullptr`.
	*
	* @tparam _Tp The value type of the vector.
	* @tparam _Allocator The allocator type used for memory management.
	* @param __il The initializer list containing the elements to be copied into the vector.
	*
	* @remark The `__il` parameter is used to initialize the vector with the elements from the initializer list. 
	* The elements are copied into the vector in the same order as they appear in the initializer list.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector( core::initializer_list< value_type > __il );

	/**
	* @brief Constructs a vector from an initializer list with a specified allocator.
	*
	* @ref (10) Constructs the container with the contents of the initializer list init. 
	*			 https://en.cppreference.com/w/cpp/container/vector/vector
	*
	* This constructor initializes a vector with the elements from the specified initializer list `__il` 
	* using a specified allocator `__a`. It sets the end-capacity pointer (`__end_capm`) to `nullptr` 
	* and assigns the allocator `__a` to the vector.
	*
	* @tparam _Tp The value type of the vector.
	* @tparam _Allocator The allocator type used for memory management.
	* @param __il The initializer list containing the elements to be copied into the vector.
	* @param __a The allocator to be used for the new vector.
	*
	* @remark The `__il` parameter is used to initialize the vector with the elements from the initializer list. 
	* The elements are copied into the vector in the same order as they appear in the initializer list. 
	* The `__a` parameter specifies the allocator to be used for memory management in the new vector.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector( core::initializer_list< value_type > __il, const allocator_type& __a );

	/**
	* @brief Move constructor for vector.
	*
	* @ref (8) Move constructor. Constructs the container with the contents of other using move semantics. 
	* Allocator is obtained by move-construction from the allocator belonging to other. 
	* After the move, other is guaranteed to be empty(). 
	* 		 https://en.cppreference.com/w/cpp/container/vector/vector
	*
	* This constructor initializes a vector by moving the contents of the specified vector `__x`. 
	* It sets the end-capacity pointer (`__end_capm`) to `nullptr` and transfers ownership of the allocator 
	* from `__x` to the new vector.
	*
	* @tparam _Tp The value type of the vector.
	* @tparam _Allocator The allocator type used for memory management.
	* @param __x The vector to be moved from.
	*
	* @remark The move constructor transfers ownership of the elements and the allocator from `__x` to the new vector. After the move, `__x` is left in a valid but unspecified state. The moved-from vector is not required to be empty.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector( vector&& __x ) LLVM_MSTL_NOEXCEPT;

	/**
	* @brief Move constructor for vector with a specified allocator.
	*
	* @ref (9) Allocator-extended move constructor. Using alloc as the allocator for the new container, 
	* moving the contents from other; if alloc != other.get_allocator(), this results in an element-wise move.
	*  (In that case, other is not guaranteed to be empty after the move.) 
	* 		 https://en.cppreference.com/w/cpp/container/vector/vector
	* 
	* This constructor initializes a vector by moving the contents of the specified vector `__x` 
	* using a specified allocator `__a`. It sets the end-capacity pointer (`__end_capm`) to `nullptr` 
	* and assigns the allocator `__a` to the new vector.
	*
	* @tparam _Tp The value type of the vector.
	* @tparam _Allocator The allocator type used for memory management.
	* @param __x The vector to be moved from.
	* @param __a The allocator to be used for the new vector.
	*
	* @remark The move constructor transfers ownership of the elements and the allocator from `__x` to the new vector. 
	* After the move, `__x` is left in a valid but unspecified state. The moved-from vector is not required to be empty. 
	* The `__a` parameter specifies the allocator to be used for memory management in the new vector.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector( vector&& __x, const core::type_identity_t< allocator_type >& __a );

	/**
	* @brief Destructor for vector.
	*
	* This destructor destroys the elements in the vector and deallocates the memory used by the vector.
	*
	* @note This destructor is available since C++20.
	*
	* @tparam _Tp The value type of the vector.
	* @tparam _Allocator The allocator type used for memory management.
	*
	* @remark The destructor invokes the destruction of each element in the vector and then deallocates the memory used by the vector. The destructor is automatically called when an object of the vector type goes out of scope or is explicitly destroyed.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 ~vector() { __destroy_vector ( *this )(); }

	/*************************************************************************************		
	 *                                                                                   *
	 *																CONSTRUCTOR END			                               *
	 *                                                                                   *
	 *************************************************************************************/
public:
	/*************************************************************************************		
	 *                                                                                   *
	 *																OPERATOR BEGIN			                               *
	 *                                                                                   *
	 *************************************************************************************/

	// TODO
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector& operator=( const vector& __x );
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector& operator=( core::initializer_list< value_type > __il );
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector& operator=( vector&& __x )
		LLVM_MSTL_NOEXCEPT_V( ( __noexcept_move_assign_container< _Allocator, __alloc_traits >::value ) );
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 reference       operator[]( size_type __n ) LLVM_MSTL_NOEXCEPT;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 const_reference operator[]( size_type __n ) const LLVM_MSTL_NOEXCEPT;

	/*************************************************************************************		
	 *                                                                                   *
	 *																	OPERATOR END			                               *
	 *                                                                                   *
	 *************************************************************************************/

public:
	/*************************************************************************************		
	 *                                                                                   *
	 *																ITERATOR BEGIN			                               *
	 *                                                                                   *
	 *************************************************************************************/

	/**
	 * @brief TODO
	 * 
	 * @return iterator 
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto begin() LLVM_MSTL_NOEXCEPT->iterator;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto begin() const LLVM_MSTL_NOEXCEPT->const_iterator;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto end() LLVM_MSTL_NOEXCEPT->iterator;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto end() const LLVM_MSTL_NOEXCEPT->const_iterator;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto rbegin() LLVM_MSTL_NOEXCEPT->iterator;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto rbegin() const LLVM_MSTL_NOEXCEPT->const_iterator;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto rend() LLVM_MSTL_NOEXCEPT->iterator;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto rend() const LLVM_MSTL_NOEXCEPT->const_iterator;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto cbegin() const LLVM_MSTL_NOEXCEPT->const_iterator;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto cend() const LLVM_MSTL_NOEXCEPT->const_iterator;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto crbegin() const LLVM_MSTL_NOEXCEPT->const_reverse_iterator;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto crend() const LLVM_MSTL_NOEXCEPT->const_reverse_iterator;

	/*************************************************************************************		
	 *                                                                                   *
	 *																	ITERATOR END			                               *
	 *                                                                                   *
	 *************************************************************************************/

private:
	/**
	* @brief Functor to destroy a vector and deallocate its memory.
	*
	* This functor is used to destroy a vector and deallocate its memory. It is typically used in exception handling
	* to ensure proper cleanup when an exception is thrown during vector construction or modification.
	*
	* The functor operator `operator()` is called to perform the destruction and deallocation. It first calls
	* `__vec.__annotate_delete()` to annotate the deletion of the vector (specific implementation-dependent behavior).
	* Then, if the vector's `__begin` pointer is not nullptr, it clears the vector by calling `__vec.__clear()`,
	* and deallocates the memory using `__alloc_traits::deallocate` with the vector's allocator and capacity.
	*/
	class __destroy_vector {
	public:
		/**
     * @brief Constructs a __destroy_vector object with a vector.
     * @param __vec The vector to be destroyed and deallocated.
     */
		LLVM_MSTL_CONSTEXPR __destroy_vector( vector& __vec )
				: __vec( __vec ) {}

		/**
     * @brief Operator function to destroy the vector and deallocate its memory.
     *
     * This operator function is called to destroy the vector and deallocate its memory.
     * It first calls `__vec.__annotate_delete()` to annotate the deletion of the vector.
     * Then, if the vector's `__begin` pointer is not nullptr, it clears the vector by calling `__vec.__clear()`,
     * and deallocates the memory using `__alloc_traits::deallocate` with the vector's allocator and capacity.
     */
		LLVM_MSTL_CONSTEXPR_SINCE_CXX20 void operator()() {
			__vec.__annotate_delete();//!<--- here is not work
			if ( __vec.__begin != nullptr ) {
				__vec.__clear();
				__alloc_traits::deallocate( __vec.__alloc(), __vec.__begin, __vec.capacity() );
			}
		}

	private:
		vector& __vec;//!<--- no `ref` cause the bug!!!
									//!<--- because the `vector&` doesn't call vector( const vector& )
									//!<--- it like this: `vector& __vec = vec`, just bind vec to __vec
									//!<--- operating `__vec` seem to operate `vec`
	};

public:
	/*************************************************************************************		
	 *                                                                                   *
	 *															FUNCTIONS BEGIN		               	                   *
	 *                                                                                   *
	 *************************************************************************************/

	/*************************************************************************************		
	 *                                                                                   *
	 *																FUNCTIONS END		               	                   *
	 *                                                                                   *
	 *************************************************************************************/
public:
	/*************************************************************************************		
	 *                                                                                   *
	 *															ELEMENT ACCESS BEGIN		               	             *
	 *                                                                                   *
	 *************************************************************************************/
	// TODO
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto at( size_type __n ) -> reference;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto at( size_type __n ) const -> const_reference;

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto front() LLVM_MSTL_NOEXCEPT->reference;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto front() const LLVM_MSTL_NOEXCEPT->const_reference;

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto back() LLVM_MSTL_NOEXCEPT->reference;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto back() const LLVM_MSTL_NOEXCEPT->const_reference;

	/**
	* @brief Returns a pointer to the underlying data of the vector.
	*
	* This member function returns a pointer to the underlying data of the vector. The pointer is obtained
	* by using the `to_address` function from the C++20 standard library. It converts the iterator `__begin`
	* to a pointer of type `value_type*`.
	*
	* @return A pointer to the underlying data of the vector.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto data() LLVM_MSTL_NOEXCEPT->value_type*;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto data() const LLVM_MSTL_NOEXCEPT->const value_type*;

	/*************************************************************************************		
	 *                                                                                   *
	 *															 ELEMENT ACCESS END		               	               *
	 *                                                                                   *
	 *************************************************************************************/
public:
	/*************************************************************************************		
	 *                                                                                   *
	 *															 	CAPACITY BEGIN			               	               *
	 *                                                                                   *
	 *************************************************************************************/

	/**
	* @brief Checks if the vector is empty.
	*
	* @ref https://en.cppreference.com/w/cpp/container/vector/empty
	* 
	* This member function checks whether the vector is empty, i.e., it has no elements.
	*
	* @tparam _Tp The value type of the vector.
	* @tparam _Allocator The allocator type used for memory management.
	* @return `true` if the vector is empty, `false` otherwise.
	*
	* @remark The function compares the internal `__begin` and `__end` pointers of the vector. 
	* If they are equal, it means the vector has no elements and it returns `true`. 
	* Otherwise, it returns `false`.
	*/
	LLVM_MSTL_NODISCARD
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto empty() const LLVM_MSTL_NOEXCEPT->bool;
	/**
	* @brief Returns the size of the vector.
	* 
	* @ref https://en.cppreference.com/w/cpp/container/vector/size 
	*
	* This member function returns the number of elements in the vector by subtracting the pointer `__begin`
	* from the pointer `__end` and casting the result to the size type `size_type`.
	*
	* @return The number of elements in the vector.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto size() const LLVM_MSTL_NOEXCEPT->size_type;
	/**
	* @brief Returns the maximum possible size of the vector.
	*
	* @ref https://en.cppreference.com/w/cpp/container/vector/max_size 
	*
	* This member function returns the maximum number of elements that the vector can hold. 
	* It takes into account the maximum size supported by the allocator and 
	* the maximum representable value of the difference type.
	*
	* @tparam _Tp The value type of the vector.
	* @tparam _Allocator The allocator type used for memory management.
	* @return The maximum possible size of the vector.
	*
	* @remark The function uses the `__alloc_traits::max_size()` function to 
	* retrieve the maximum size supported by the allocator. It compares this value with 
	* the maximum representable value of the difference type 
	* (`core::numeric_limits<difference_type>::max()`) and returns the smaller value. 
	* This ensures that the maximum possible size of the vector is not exceeded.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto max_size() const LLVM_MSTL_NOEXCEPT->size_type;
	/**
	* @brief Reserves storage space for at least the specified number of elements.
	* 
	* @ref https://en.cppreference.com/w/cpp/container/vector/reserve 
	*
	* This member function reserves storage space in the vector to accommodate at least the specified number of elements. 
	* If the requested capacity is greater than the current capacity of the vector, reallocation occurs.
	*  However, if the requested capacity is greater than the maximum size supported by the vector, 
	* a length_error exception is thrown.
	*
	* @tparam _Tp The value type of the vector.
	* @tparam _Allocator The allocator type used for memory management.
	* @param __n The minimum capacity to reserve.
	*
	* @remark The function first checks if the requested capacity (`__n`) is greater than the current capacity of the vector. 
	* If so, it proceeds to check if the requested capacity is greater than the maximum size supported by the vector using the `max_size()` function. 
	* If the requested capacity is greater than the maximum size, a `length_error` exception is thrown.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto reserve( size_type __n );
	/**
	* @brief Returns the capacity of the vector.
	*
	* @ref https://en.cppreference.com/w/cpp/container/vector/capacity  
	*
	* This member function returns the capacity of the vector, which represents the maximum number of elements
	* that the vector can hold without reallocating memory. The capacity is calculated by subtracting the pointer
	* returned by `__end_cap()` from the pointer `__begin` and casting the result to the size type `size_type`.
	*
	* @return The capacity of the vector.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto capacity() const LLVM_MSTL_NOEXCEPT->size_type;
	/**
	* @brief Reduces the capacity of the vector to fit its size.
	*
	* @ref https://en.cppreference.com/w/cpp/container/vector/shrink_to_fit
	*
	* This member function reduces the capacity of the vector to match its size. 
	* If the current capacity is greater than the size of the vector, reallocation occurs to release the unused memory.
	*
	* @tparam _Tp The value type of the vector.
	* @tparam _Allocator The allocator type used for memory management.
	*
	* @remark The function first checks if the current capacity (`capacity()`) is greater than the size (`size()`) of the vector. 
	* If so, it creates a new split buffer (`__v`) with the size and size as the capacity, and the allocator. 
	* It then swaps the internal circular buffer of the vector with the new split buffer using the `__swap_out_circular_buffer()` function.
	*
	* @note The `__swap_out_circular_buffer()` function is not defined in the provided code snippet and should be implemented elsewhere.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto shrink_to_fit() LLVM_MSTL_NOEXCEPT;

	/*************************************************************************************		
	 *                                                                                   *
	 *															 		CAPACITY END			               	               *
	 *                                                                                   *
	 *************************************************************************************/

public:
	/*************************************************************************************		
	 *                                                                                   *
	 *															 	MODIFIERS BEGIN			               	               *
	 *                                                                                   *
	 *************************************************************************************/
	// TODO
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto clear() LLVM_MSTL_NOEXCEPT;

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto insert( const_iterator __position, const_reference __x ) -> iterator;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto insert( const_iterator __position, value_type&& __x ) -> iterator;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto insert( const_iterator __position, size_type __n, const_reference __x ) -> iterator;
	template <
		typename _InputIterator,
		core::enable_if_t<
			__is_exactly_cpp17_input_iterator< _InputIterator >::value &&
				core::is_constructible_v<
					value_type,
					typename core::iterator_traits< _InputIterator >::reference >,
			int > = 0 >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto insert( const_iterator __position, _InputIterator __fist, _InputIterator __last ) -> iterator;
	template <
		typename _ForwardIterator,
		core::enable_if_t<
			__is_cpp17_forward_iterator< _ForwardIterator >::value &&
				core::is_constructible_v<
					value_type,
					typename core::iterator_traits< _ForwardIterator >::reference >,
			int > = 0 >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto insert( const_iterator __position, _ForwardIterator __fist, _ForwardIterator __last ) -> iterator;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto insert( const_iterator __position, core::initializer_list< value_type > __il ) -> iterator;

	template < typename... _Args >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto emplace( const_iterator __position, _Args&&... __args );

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto erase( const_iterator __position ) -> iterator;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto erase( const_iterator __fist, const_iterator __last ) -> iterator;

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto push_back( const_reference __x );
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto push_back( value_type&& __x );

	template < typename... _Args >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto emplace_back( _Args&&... __args ) -> reference;

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto pop_back();

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto resize( size_type __sz );
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto resize( size_type __sz, const_reference __x );

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto assign( size_type __n, const_reference __u );
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto assign( core::initializer_list< value_type > __il );
	template <
		typename _InputIterator,
		core::enable_if_t<
			__is_exactly_cpp17_input_iterator< _InputIterator >::value &&
				core::is_constructible_v<
					value_type,
					typename core::iterator_traits< _InputIterator >::reference >,
			int > = 0 >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto assign( _InputIterator __first, _InputIterator __last );

	template <
		typename _ForwardIterator,
		core::enable_if_t<
			__is_cpp17_forward_iterator< _ForwardIterator >::value &&
				core::is_constructible_v<
					value_type,
					typename core::iterator_traits< _ForwardIterator >::reference >,
			int > = 0 >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto assign( _ForwardIterator __first, _ForwardIterator __last );

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto swap( vector& ) LLVM_MSTL_NOEXCEPT;

	/*************************************************************************************		
	 *                                                                                   *
	 *															 		MODIFIERS END			               	               *
	 *                                                                                   *
	 *************************************************************************************/

private:
	/*************************************************************************************		
	 *                                                                                   *
	 *																HELPER BEGIN		               	                   *
	 *                                                                                   *
	 *************************************************************************************/

	/**
	* @brief Allocates memory for at least `__n` elements.
	*
	* This function allocates memory using the allocator `__alloc()` for at least `__n` elements.
	* If `__n` exceeds the maximum size that can be allocated, it throws a `length_error` exception.
	* The allocated memory is stored in the `__begin` and `__end` member variables, with `__begin`
	* and `__end` pointing to the beginning of the allocated memory. The `__end_cap()` member function
	* is modified to point to the last position of the allocated memory.
	*
	* @param __n The minimum number of elements to allocate memory for.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __vallocate( size_type __n ) -> void;

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __vdeallocate() LLVM_MSTL_NOEXCEPT;

	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __recommend( size_type __new_size ) const -> size_type;

	/**
	* @brief Swap out the circular buffer with another split buffer by performing move construction.
	*
	* This member function is used to swap out the circular buffer of the vector with another split buffer by performing move construction.
	*
	* @tparam _Tp The value type of the vector.
	* @tparam _Allocator The allocator type of the vector.
	* @param __v The split buffer to swap with.
	*
	* @remark The function first calls `__annotate_delete()` to indicate that the deletion of the circular buffer is annotated but no actual work is done.
	*
	* It then defines a reverse iterator type `_RevIter` using `core::reverse_iterator` to iterate over the circular buffer in reverse order.
	*
	* The function uses `__uninitialized_allocator_move_if_noexcept()` to move-construct the elements from the circular buffer into the split buffer. 
	* It passes the allocator reference `__alloc()`, and the reverse iterators for the circular buffer and the split buffer. 
	* The move construction is performed only if the move constructor of the value type is declared as `noexcept`. 
	* The resulting split buffer is assigned to `__v.__begin`.
	*
	* The function then swaps the pointers `__begin`, `__end`, and `__end_cap()` of the vector 
	* with the corresponding pointers in the split buffer using `core::swap()`. 
	* This effectively swaps the circular buffer with the split buffer.
	*
	* Finally, the function sets `__v.__first` to `__v.__begin` and calls `__annotate_new(size())` to annotate the creation of new elements, 
	* but no actual work is done.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __swap_out_circular_buffer( __split_buffer< value_type, allocator_type& >& __v );
	/**
	* @brief Swap out the circular buffer with another split buffer at a specified pointer by performing move construction.
	*
	* This member function is used to swap out the circular buffer of the vector with another split buffer at a specified pointer by performing move construction.
	*
	* @tparam _Tp The value type of the vector.
	* @tparam _Allocator The allocator type of the vector.
	* @param __v The split buffer to swap with.
	* @param __p The pointer indicating the position of the split.
	* @return The pointer to the beginning of the original circular buffer.
	*
	* @remark The function first calls `__annotate_delete()` to indicate that the deletion of the circular buffer is annotated but no actual work is done.
	*
	* It then declares a pointer `__r` to store the beginning of the original circular buffer.
	* 
	* The function defines a reverse iterator type `_RevIter` using `core::reverse_iterator` to iterate over the circular buffer and split buffer in reverse order.
	*
	* The function uses `__uninitialized_allocator_move_if_noexcept()` twice to move-construct the elements from the circular buffer 
	* and the split buffer into the respective locations in the split buffer and the circular buffer. It passes the allocator reference `__alloc()`, 
	* and the reverse iterators for the circular buffer, split buffer, and the specified pointer `__p`. 
	* The move construction is performed only if the move constructor of the value type is declared as `noexcept`. 
	* The resulting split buffer is assigned to `__v.__begin`, and the resulting circular buffer is assigned to `__v.__end`.
	*
	* The function then swaps the pointers `__begin`, `__end`, and `__end_cap()` of the vector 
	* with the corresponding pointers in the split buffer using `core::swap()`. This effectively swaps the circular buffer with the split buffer.
	*
	* Finally, the function sets `__v.__first` to `__v.__begin`, calls `__annotate_new(size())` to annotate the creation of new elements, 
	* and returns the pointer `__r` which points to the beginning of the original circular buffer.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __swap_out_circular_buffer( __split_buffer< value_type, allocator_type& >& __v, pointer __p ) -> pointer;

	/**
	* @brief Constructs a specified number of elements at the end of the vector.
	* 
	* This function constructs a specified number of elements at the end of the vector, effectively increasing its size.
	*
	* @tparam _Tp The type of elements in the vector.
	* @tparam _Allocator The allocator type used for memory management.
	* @param __n The number of elements to construct.
	* @return The new end iterator of the vector after construction.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __construct_at_end( size_type __n );

	/**
	* @brief Constructs elements at the end of the vector with a specified value.
	*
	* @param __n The number of elements to be constructed.
	* @param __x The value to be assigned to each constructed element.
	*
	* This function constructs `__n` elements at the end of the vector with the specified value (`__x`).
	*
	* It uses placement new to construct elements at the specified positions. The `__n` elements are
	* constructed starting from the current end position of the vector.
	*
	* If an exception is thrown during the construction, the previously constructed elements are
	* destroyed and the exception is rethrown.
	*
	* @return A pointer to the newly constructed element after the construction is complete.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __construct_at_end( size_type __n, const_reference __x );

	template <
		typename _ForwardIterator,
		core::enable_if_t<
			__is_cpp17_forward_iterator< _ForwardIterator >::value,
			int > = 0 >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __construct_at_end( _ForwardIterator __first, _ForwardIterator __last, size_type __n );

	template < typename... _Args >
	LLVM_MSTL_TEMPLATE_INLINE LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __emplace_back_slow_path( _Args&&... __args );

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __annotate_contiguous_container( const void*, const void*, const void*, const void* ) const LLVM_MSTL_NOEXCEPT {}

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __annotate_new( size_type __current_size ) const LLVM_MSTL_NOEXCEPT {
		__annotate_contiguous_container( data(), data() + capacity(), data() + capacity(), data() + __current_size );
	}

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __annotate_delete() const LLVM_MSTL_NOEXCEPT {
		__annotate_contiguous_container( data(), data() + capacity(), data() + size(), data() + capacity() );
	}
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __annotate_increase( size_type __n ) const LLVM_MSTL_NOEXCEPT {
		static_cast< void >( __n );
		// __annotate_contiguous_container( data() + size(), data() + size() + __n );
	}
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __annotate_shrink( size_type __n ) const LLVM_MSTL_NOEXCEPT {
		static_cast< void >( __n );
	}

	/**
		* @brief A transaction for reserving space at the end of a vector.
		*
		* @note When drop the live life, call the `~_ConstructTransaction()` 
		* to update the new `__end` pointer tp the right position
		*
		*/
	struct _ConstructTransaction {
		/**
     * @brief Constructs a _ConstructTransaction object.
     * 
     * This constructor reserves space at the end of the vector for future elements.
     *
     * @param __v The vector to operate on.
     * @param __n The number of elements to reserve.
     */
		LLVM_MSTL_CONSTEXPR_SINCE_CXX20 explicit _ConstructTransaction( vector& __v, size_type __n )
				: __v( __v )
				, __pos( __v.__end )
				, __new_end( __v.__end + __n ) {}

		/**
     * @brief Destructs the _ConstructTransaction object.
     * 
     * This destructor restores the end position of the vector to the original value.
     */
		LLVM_MSTL_CONSTEXPR_SINCE_CXX20 ~_ConstructTransaction() {
			__v.__end = __pos;
		}

		vector&             __v;      //<--- The vector being operated on
		pointer             __pos;    //<--- The `original end position` of the vector.
		const_pointer const __new_end;//<--- The `new end position` of the vector after reserving space.
	};

	/**
	* @brief Constructs a single element at the end of the vector.
	* 
	* This function constructs a single element at the end of the vector, effectively increasing its size by one.
	* 
	* @tparam _Args Variadic template parameter pack for the types of arguments used for construction.
	* @param __args Arguments to be forwarded for element construction.
	* @return The new end iterator of the vector after construction.
 	*/
	template < class... _Args >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __construct_one_at_end( _Args&&... __args ) {
		_ConstructTransaction __tx( *this, 1 );
		__alloc_traits::construct( this->__alloc(), core::to_address( __tx.__pos ), std::forward< _Args >( __args )... );
		++__tx.__pos;
	}

	/**
	 * @brief get the allocator object for vector
	 * 
	 * @return allocator_type& the allocator
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __alloc() LLVM_MSTL_NOEXCEPT->allocator_type& {
		return this->__end_capm.second();
	}

	/**
	 * @brief get the allocator object for vector
	 * 
	 * @return allocator_type& the allocator
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __alloc() const LLVM_MSTL_NOEXCEPT->const allocator_type& {
		return this->__end_capm.second();
	}

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __end_cap() LLVM_MSTL_NOEXCEPT->pointer& {
		return this->__end_capm.first();
	}

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __end_cap() const LLVM_MSTL_NOEXCEPT->const pointer& {
		return this->__end_capm.first();
	}

	/**
	 * @brief TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __make_iter( pointer __p )
		LLVM_MSTL_NOEXCEPT->iterator { return iterator( this, __p ); }
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __make_iter( pointer __p ) const
		LLVM_MSTL_NOEXCEPT->const_iterator { return const_iterator( this, __p ); }


	/**
	* @brief Clears the vector by destructing elements from `__end` to the `__begin`.
	*
	* This member function clears the vector by destructing elements from `__end` to the `__begin`. It calls
	* `__base_destruct_at_end` to perform the destruction.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __clear() LLVM_MSTL_NOEXCEPT {
		__base_destruct_at_end( this->__begin );
	}

	/**
	* @brief Destructs elements from `__end` to `__new_last`.
	*
	* This member function destructs elements from `__end` to `__new_last`. It is used by `__clear` function.
	* It iterates from `__end` to `__new_last`, destructing elements using `__alloc_traits::destroy`.
	* Finally, we use `this->__end = __new_last` to indicates the end of the destruction
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __base_destruct_at_end( pointer __new_last ) LLVM_MSTL_NOEXCEPT {
		pointer __soon_to_be_end = this->__end;
		while ( __new_last != __soon_to_be_end )
			__alloc_traits::destroy( __alloc(), core::to_address( --__soon_to_be_end ) );
		this->__end = __new_last;
	}

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __move_range( pointer __from_s, pointer __from_e, pointer __to );
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __move_assign( vector& __c, core::true_type )
		LLVM_MSTL_NOEXCEPT_V( core::is_nothrow_move_assignable_v< allocator_type > ) -> void;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __move_assign( vector& __c, core::false_type )
		LLVM_MSTL_NOEXCEPT_V( core::is_nothrow_move_assignable_v< allocator_type > ) -> void;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __move_assign_alloc( vector& __c )
		LLVM_MSTL_NOEXCEPT_V(
			!__alloc_traits::propagate_on_container_move_assignment::value ||
			core::is_nothrow_move_assignable_v< allocator_type > );

	/**
	 * @brief TODO
	 * 
	 * @param __c 
	 * @return LLVM_MSTL_CONSTEXPR_SINCE_CXX20 
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __copy_assign_alloc( const vector& __c ) {
		__copy_assign_alloc(
			__c, core::integral_constant<
						 bool,
						 __alloc_traits::propagate_on_container_copy_assignment::value >() );
	}

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __copy_assign_alloc( const vector& __c, core::true_type ) {
		if ( __alloc() != __c.__alloc() ) __vdeallocate();
		__alloc() = __c.__alloc();
	}

	LLVM_MSTL_NORETURN auto __throw_length_error() const {
		nya::__throw_length_error( "vector" );
	}

	LLVM_MSTL_NORETURN auto __throw_out_of_range() const {
		nya::__throw_out_of_range( "vector" );
	}

	/*************************************************************************************		
	 *                                                                                   *
	 *																	HELPER END		               	                   *
	 *                                                                                   *
	 *************************************************************************************/
private:
	pointer                                      __begin    = nullptr;//<--- point the `first` element in vector
	pointer                                      __end      = nullptr;//<--- point the last element in vector(not really `last`, `the last afther the last one`)
	__compressed_pair< pointer, allocator_type > __end_capm =         //<--- use less memory to store the end point and allocator
																																		//<--- the `cap.first` is the `end`, point the `last element afther the last one`
																																		//<--- the `cap.second` is the `allocator`, use to manage memory's alloc and release
		__compressed_pair< pointer, allocator_type >( nullptr, __default_init_tag() );
};


/*************************************************************************************		
 *                                                                                   *
 *															CONSTRUCTOR BEGIN		                                 *
 *                                                                                   *
 *************************************************************************************/
template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector< _Tp, _Allocator >::vector( size_type __n ) {
	// create a RAII exception guard
	// if `exception happend`, cause a non-normal exit, it will automatic drop by calling `__destroy_vector`
	auto __guard = __make_exception_guard( __destroy_vector( *this ) );
	if ( __n > 0 ) {
		__vallocate( __n );       //<--- allocate a memmory and set `__begin`、`__end` and `__end_capm`.
															//<--- it just allocate the memory, doesn't update the `__end`
		__construct_at_end( __n );//!<--- `__vallocate` dosen't make `__end` point the last position
															//!<--- `__construct_at_end` really make `__end` point the right position(the last ather the last one)
	}
	__guard.__complete();
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector< _Tp, _Allocator >::vector( size_type __n, const allocator_type& __a )
		: __end_capm( nullptr, __a ) {
	auto __guard = __make_exception_guard( __destroy_vector( *this ) );
	if ( __n > 0 ) {
		__vallocate( __n );
		__construct_at_end( __n );
	}
	__guard.__complete();
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector< _Tp, _Allocator >::vector( size_type __n, const value_type& __x ) {
	auto __guard = __make_exception_guard( __destroy_vector( *this ) );
	if ( __n > 0 ) {
		__vallocate( __n );
		__construct_at_end( __n, __x );
	}
	__guard.__complete();
}

template < typename _Tp, typename _Allocator >
template <
	typename _InputIterator,
	core::enable_if_t<
		__is_exactly_cpp17_input_iterator< _InputIterator >::value &&
			core::is_constructible_v<
				_Tp,
				typename core::iterator_traits< _InputIterator >::reference >,
		int > >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector< _Tp, _Allocator >::vector( _InputIterator __first, _InputIterator __last ) {
	auto __guard = __make_exception_guard( __destroy_vector( *this ) );
	for ( ; __first != __last; ++__first )
		emplace_back( *__first );
	__guard.__complete();
}

template < typename _Tp, typename _Allocator >
template <
	typename _InputIterator,
	core::enable_if_t<
		__is_exactly_cpp17_input_iterator< _InputIterator >::value &&
			core::is_constructible_v<
				_Tp,
				typename core::iterator_traits< _InputIterator >::reference >,
		int > >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector< _Tp, _Allocator >::vector(
	_InputIterator __first, _InputIterator __last, const allocator_type& __a )
		: __end_capm( nullptr, __a ) {
	auto __guard = __make_exception_guard( __destroy_vector( *this ) );
	for ( ; __first != __last; ++__first )
		emplace_back( *__first );
	__guard.__complete();
}

template < typename _Tp, typename _Allocator >
template <
	typename _ForwardIterator,
	core::enable_if_t<
		__is_cpp17_forward_iterator< _ForwardIterator >::value &&
			core::is_constructible_v<
				_Tp,
				typename core::iterator_traits< _ForwardIterator >::reference >,
		int > >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector< _Tp, _Allocator >::vector( _ForwardIterator __first, _ForwardIterator __last ) {
	auto __guard = __make_exception_guard( __destroy_vector( *this ) );
	//<--- caculate the length of input iterator
	size_type __n = static_cast< size_type >( core::distance( __first, __last ) );
	if ( __n > 0 ) {
		__vallocate( __n );
		__construct_at_end( __first, __last, __n );
	}
	__guard.__complete();
}

template < typename _Tp, typename _Allocator >
template <
	typename _ForwardIterator,
	core::enable_if_t<
		__is_cpp17_forward_iterator< _ForwardIterator >::value &&
			core::is_constructible_v<
				_Tp,
				typename core::iterator_traits< _ForwardIterator >::reference >,
		int > >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector< _Tp, _Allocator >::vector(
	_ForwardIterator __first, _ForwardIterator __last, const allocator_type& __a )
		: __end_capm( nullptr, __a ) {
	auto      __guard = __make_exception_guard( __destroy_vector( *this ) );
	size_type __n     = static_cast< size_type >( core::distance( __first, __last ) );
	if ( __n > 0 ) {
		__vallocate( __n );
		__construct_at_end( __first, __last, __n );
	}
	__guard.__complete();
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector< _Tp, _Allocator >::vector( const vector& __x )
		: __end_capm( nullptr, __alloc_traits::select_on_container_copy_construction( __x.__alloc() ) ) {
	auto      __guard = __make_exception_guard( __destroy_vector( *this ) );
	size_type __n     = __x.size();
	if ( __n > 0 ) {
		__vallocate( __n );
		__construct_at_end( __x.__begin, __x.__end, __n );
	}
	__guard.__complete();
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector< _Tp, _Allocator >::vector(
	const vector& __x, const core::type_identity_t< allocator_type >& __a )
		: __end_capm( nullptr, __a ) {
	auto      __guard = __make_exception_guard( __destroy_vector( *this ) );
	size_type __n     = __x.size();
	if ( __n > 0 ) {
		__vallocate( __n );
		__construct_at_end( __x.__begin, __x.__end, __n );
	}
	__guard.__complete();
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector< _Tp, _Allocator >::vector( core::initializer_list< value_type > __il ) {
	auto __guard = __make_exception_guard( __destroy_vector( *this ) );
	if ( __il.size() > 0 ) {
		__vallocate( __il.size() );
		__construct_at_end( __il.begin(), __il.end(), __il.size() );
	}
	__guard.__complete();
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector< _Tp, _Allocator >::vector(
	core::initializer_list< value_type > __il, const allocator_type& __a )
		: __end_capm( nullptr, __a ) {
	auto __guard = __make_exception_guard( __destroy_vector( *this ) );
	if ( __il.size() > 0 ) {
		__vallocate( __il.size() );
		__construct_at_end( __il.begin(), __il.end(), __il.size() );
	}
	__guard.__complete();
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 LLVM_MSTL_TEMPLATE_INLINE
vector< _Tp, _Allocator >::vector( vector&& __x ) LLVM_MSTL_NOEXCEPT
		: __end_capm( nullptr, core::move( __x.__alloc() ) ) {
	this->__begin     = __x.__begin;
	this->__end       = __x.__end;
	this->__end_cap() = __x.__end_cap();
	__x.__begin = __x.__end = __x.__end_cap() = nullptr;
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 LLVM_MSTL_TEMPLATE_INLINE
vector< _Tp, _Allocator >::vector( vector&& __x, const core::type_identity_t< allocator_type >& __a )
		: __end_capm( nullptr, __a ) {
	this->__begin     = __x.__begin;
	this->__end       = __x.__end;
	this->__end_cap() = __x.__end_cap();
	__x.__begin = __x.__end = __x.__end_cap() = nullptr;
}

/*************************************************************************************		
 *                                                                                   *
 *																CONSTRUCTOR END		                                 *
 *                                                                                   *
 *************************************************************************************/

/*************************************************************************************		
 *                                                                                   *
 *																OPERATOR BEGIN			                               *
 *                                                                                   *
 *************************************************************************************/

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE
		vector< _Tp, _Allocator >&
		vector< _Tp, _Allocator >::operator=( const vector& __x ) {
	if ( this != core::addressof( __x ) ) {
		__copy_assign_alloc( __x );
		assign( __x.__begin, __x.__end );
	}
	return *this;
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE
		vector< _Tp, _Allocator >&
		vector< _Tp, _Allocator >::operator=( core::initializer_list< value_type > __il ) {
	assign( __il.begin(), __il.end() );
	return *this;
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE
		vector< _Tp, _Allocator >&
		vector< _Tp, _Allocator >::operator=( vector&& __x )
			LLVM_MSTL_NOEXCEPT_V(
				( __noexcept_move_assign_container< _Allocator, __alloc_traits >::value ) ) {
	__move_assign(
		__x,
		core::integral_constant<
			bool,
			__alloc_traits::propagate_on_container_move_assignment::value >() );
	return *this;
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 LLVM_MSTL_TEMPLATE_INLINE
	typename vector< _Tp, _Allocator >::reference
	vector< _Tp, _Allocator >::operator[]( size_type __n ) LLVM_MSTL_NOEXCEPT {
	// static_assert( __n < size(), "vector[] index out of bounds" );
	if ( __n >= size() ) {
		spdlog::error( "vector[] index out of bounds, __n[{}] >= size()[{}]", __n, size() );
		__throw_out_of_range();
	}
	// LLVM_MSTL_ERROR( "vector[] index out of bounds, __n[{}] >= size()[{}]", __n, size() );
	return this->__begin[ __n ];
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 LLVM_MSTL_TEMPLATE_INLINE
	typename vector< _Tp, _Allocator >::const_reference
	vector< _Tp, _Allocator >::operator[]( size_type __n ) const LLVM_MSTL_NOEXCEPT {
	// static_assert( __n < size(), "vector[] index out of bounds" );
	if ( __n >= size() ) {
		spdlog::error( "vector[] index out of bounds, __n[{}] >= size()[{}]", __n, size() );
		__throw_out_of_range();
	}
	return this->__begin[ __n ];
}

/**
 * @brief Equality comparison operator for vectors.
 *
 * This operator compares two vectors `__x` and `__y` for equality. It returns `true` 
 * if the vectors have the same size and their corresponding elements are equal, and `false` otherwise.
 *
 * @tparam _Tp The value type of the vectors.
 * @tparam _Allocator The allocator type used for memory management.
 * @param __x The first vector to compare.
 * @param __y The second vector to compare.
 * @return `true` if the vectors are equal, `false` otherwise.
 *
 * @remark The operator compares the sizes of the vectors using their `size()` member functions. 
 * If the sizes are not equal, it returns `false`. Otherwise, it uses the `core::equal()` function to 
 * compare the elements of the vectors. The `core::equal()` function compares the elements of 
 * the ranges [__x.begin(), __x.end()) and [__y.begin(), __y.end()) for equality. 
 * If all corresponding elements are equal, the operator returns `true`; otherwise, it returns `false`.
 */
template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE bool
	operator==( const vector< _Tp, _Allocator >& __x, const vector< _Tp, _Allocator >& __y ) {
	const typename vector< _Tp, _Allocator >::size_type __sz = __x.size();
	return __sz == __y.size() &&
				 core::equal( __x.begin(), __x.end(), __y.begin() );
}

/**
 * @brief Inequality comparison operator for vectors.
 *
 * This operator compares two vectors `__x` and `__y` for inequality. It returns `true` 
 * if the vectors are not equal, and `false` otherwise.
 *
 * @tparam _Tp The value type of the vectors.
 * @tparam _Allocator The allocator type used for memory management.
 * @param __x The first vector to compare.
 * @param __y The second vector to compare.
 * @return `true` if the vectors are not equal, `false` otherwise.
 */
template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE bool
	operator!=( const vector< _Tp, _Allocator >& __x, const vector< _Tp, _Allocator >& __y ) {
	return !( __x == __y );
}

/**
 * @brief Less-than comparison operator for vectors.
 *
 * This operator compares two vectors `__x` and `__y` lexicographically. It returns `true` 
 * if `__x` is lexicographically less than `__y`, and `false` otherwise.
 *
 * @tparam _Tp The value type of the vectors.
 * @tparam _Allocator The allocator type used for memory management.
 * @param __x The first vector to compare.
 * @param __y The second vector to compare.
 * @return `true` if `__x` is lexicographically less than `__y`, `false` otherwise.
 *
 * @remark The operator uses the `core::lexicographical_compare()` function to 
 * perform the lexicographical comparison between the elements of the vectors. 
 * It compares the ranges [__x.begin(), __x.end()) and [__y.begin(), __y.end()). 
 * If __x is lexicographically less than __y, it returns `true`; otherwise, it returns `false`.
 */
template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE bool
	operator<( const vector< _Tp, _Allocator >& __x, const vector< _Tp, _Allocator >& __y ) {
	return core::lexicographical_compare( __x.begin(), __x.end(), __y.begin(), __y.end() );
}

/**
 * @brief Greater-than comparison operator for vectors.
 *
 * This operator compares two vectors `__x` and `__y` lexicographically. It returns `true` 
 * if `__x` is lexicographically greater than `__y`, and `false` otherwise.
 *
 * @tparam _Tp The value type of the vectors.
 * @tparam _Allocator The allocator type used for memory management.
 * @param __x The first vector to compare.
 * @param __y The second vector to compare.
 * @return `true` if `__x` is lexicographically greater than `__y`, `false` otherwise.
 */
template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE bool
	operator>( const vector< _Tp, _Allocator >& __x, const vector< _Tp, _Allocator >& __y ) {
	return __y < __x;
}

/**
 * @brief Less-than or equal-to comparison operator for vectors.
 *
 * This operator compares two vectors `__x` and `__y` lexicographically. It returns `true` 
 * if `__x` is lexicographically less than or equal to `__y`, and `false` otherwise.
 *
 * @tparam _Tp The value type of the vectors.
 * @tparam _Allocator The allocator type used for memory management.
 * @param __x The first vector to compare.
 * @param __y The second vector to compare.
 * @return `true` if `__x` is lexicographically less than or equal to `__y`, `false` otherwise.
 */
template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE bool
	operator<=( const vector< _Tp, _Allocator >& __x, const vector< _Tp, _Allocator >& __y ) {
	return !( __y < __x );
}

/**
 * @brief Greater-than or equal-to comparison operator for vectors.
 *
 * This operator compares two vectors `__x` and `__y` lexicographically. It returns `true` 
 * if `__x` is lexicographically greater than or equal to `__y`, and `false` otherwise.
 *
 * @tparam _Tp The value type of the vectors.
 * @tparam _Allocator The allocator type used for memory management.
 * @param __x The first vector to compare.
 * @param __y The second vector to compare.
 * @return `true` if `__x` is lexicographically greater than or equal to `__y`, `false` otherwise.
 */
template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE bool
	operator>=( const vector< _Tp, _Allocator >& __x, const vector< _Tp, _Allocator >& __y ) {
	return !( __x < __y );
}

/*************************************************************************************		
 *                                                                                   *
 *																	OPERATOR END			                               *
 *                                                                                   *
 *************************************************************************************/

/*************************************************************************************		
 *                                                                                   *
 *																ITERATOR BEGIN			                               *
 *                                                                                   *
 *************************************************************************************/

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	vector< _Tp, _Allocator >::begin()
		LLVM_MSTL_NOEXCEPT->typename vector< _Tp, _Allocator >::iterator {
	return __make_iter( this->__begin );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	vector< _Tp, _Allocator >::begin() const
	LLVM_MSTL_NOEXCEPT->typename vector< _Tp, _Allocator >::const_iterator {
	return __make_iter( this->__begin );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	vector< _Tp, _Allocator >::end()
		LLVM_MSTL_NOEXCEPT->typename vector< _Tp, _Allocator >::iterator {
	return __make_iter( this->__end );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	vector< _Tp, _Allocator >::end() const
	LLVM_MSTL_NOEXCEPT->typename vector< _Tp, _Allocator >::const_iterator {
	return __make_iter( this->__end );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	vector< _Tp, _Allocator >::rbegin()
		LLVM_MSTL_NOEXCEPT->typename vector< _Tp, _Allocator >::iterator {
	return reverse_iterator( end() );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	vector< _Tp, _Allocator >::rbegin() const
	LLVM_MSTL_NOEXCEPT->typename vector< _Tp, _Allocator >::const_iterator {
	return const_reverse_iterator( end() );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	vector< _Tp, _Allocator >::rend()
		LLVM_MSTL_NOEXCEPT->typename vector< _Tp, _Allocator >::iterator {
	return reverse_iterator( begin() );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	vector< _Tp, _Allocator >::rend() const
	LLVM_MSTL_NOEXCEPT->typename vector< _Tp, _Allocator >::const_iterator {
	return const_reverse_iterator( begin() );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	vector< _Tp, _Allocator >::cbegin() const
	LLVM_MSTL_NOEXCEPT->typename vector< _Tp, _Allocator >::const_iterator {
	return begin();
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	vector< _Tp, _Allocator >::cend() const
	LLVM_MSTL_NOEXCEPT->typename vector< _Tp, _Allocator >::const_iterator {
	return end();
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	vector< _Tp, _Allocator >::crbegin() const
	LLVM_MSTL_NOEXCEPT->typename vector< _Tp, _Allocator >::const_reverse_iterator {
	return rbegin();
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	vector< _Tp, _Allocator >::crend() const
	LLVM_MSTL_NOEXCEPT->typename vector< _Tp, _Allocator >::const_reverse_iterator {
	return rend();
}


/*************************************************************************************		
 *                                                                                   *
 *																	ITERATOR END			                               *
 *                                                                                   *
 *************************************************************************************/

/*************************************************************************************		
 *                                                                                   *
 *															ELEMENT ACCESS BEGIN		               	             *
 *                                                                                   *
 *************************************************************************************/

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto vector< _Tp, _Allocator >::at( size_type __n )
	-> typename vector< _Tp, _Allocator >::reference {
	if ( __n > size() ) this->__throw_out_of_range();
	return this->__begin[ __n ];
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto vector< _Tp, _Allocator >::at( size_type __n ) const
	-> typename vector< _Tp, _Allocator >::const_reference {
	if ( __n > size() ) this->__throw_out_of_range();
	return this->__begin[ __n ];
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_TEMPLATE_INLINE
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
	vector< _Tp, _Allocator >::front() LLVM_MSTL_NOEXCEPT
		->vector< _Tp, _Allocator >::reference {
	static_assert( !empty(), "front() called on an empty vector" );
	return *this->__begin;
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_TEMPLATE_INLINE
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
	vector< _Tp, _Allocator >::front() const LLVM_MSTL_NOEXCEPT
		->vector< _Tp, _Allocator >::const_reference {
	static_assert( !empty(), "front() called on an empty vector" );
	return *this->__begin;
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_TEMPLATE_INLINE
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
	vector< _Tp, _Allocator >::back() LLVM_MSTL_NOEXCEPT
		->vector< _Tp, _Allocator >::reference {
	// static_assert( !empty(), "back() called on an empty vector" );
	if ( empty() ) {
		spdlog::error( "back() called on an empty vector" );
		__throw_length_error();
	}
	return *( this->__end - 1 );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_TEMPLATE_INLINE
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
	vector< _Tp, _Allocator >::back() const LLVM_MSTL_NOEXCEPT
		->vector< _Tp, _Allocator >::const_reference {
	static_assert( !empty(), "back() called on an empty vector" );
	return *( this->__end - 1 );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_TEMPLATE_INLINE
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
	vector< _Tp, _Allocator >::data() LLVM_MSTL_NOEXCEPT
		->vector< _Tp, _Allocator >::value_type* {
	return core::to_address( this->__begin );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_TEMPLATE_INLINE
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
	vector< _Tp, _Allocator >::data() const LLVM_MSTL_NOEXCEPT
		->const vector< _Tp, _Allocator >::value_type* {
	return core::to_address( this->__begin );
}

/*************************************************************************************		
 *                                                                                   *
 *																ELEMENT ACCESS END		               	             *
 *                                                                                   *
 *************************************************************************************/

/*************************************************************************************		
 *                                                                                   *
 *															 	CAPACITY BEGIN			               	               *
 *                                                                                   *
 *************************************************************************************/

template < typename _Tp, typename _Allocator >
LLVM_MSTL_NODISCARD LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	vector< _Tp, _Allocator >::empty() const LLVM_MSTL_NOEXCEPT->bool {
	return this->__begin == this->__end;
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 LLVM_MSTL_TEMPLATE_INLINE auto
vector< _Tp, _Allocator >::size() const LLVM_MSTL_NOEXCEPT
	->vector< _Tp, _Allocator >::size_type {
	return static_cast< size_type >( this->__end - this->__begin );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
vector< _Tp, _Allocator >::max_size() const LLVM_MSTL_NOEXCEPT
	->vector< _Tp, _Allocator >::size_type {
	return core::min< size_type >(
		__alloc_traits::max_size( this->__alloc() ),
		core::numeric_limits< difference_type >::max() );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto vector< _Tp, _Allocator >::reserve( size_type __n ) {
	if ( __n > capacity() ) {
		if ( __n > max_size() ) {
			this->__throw_length_error();
		}
		allocator_type&                               __a = this->__alloc();
		__split_buffer< value_type, allocator_type& > __v( __n, size(), __a );
		__swap_out_circular_buffer( __v );
	}
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 LLVM_MSTL_TEMPLATE_INLINE auto
vector< _Tp, _Allocator >::capacity() const LLVM_MSTL_NOEXCEPT
	->vector< _Tp, _Allocator >::size_type {
	return static_cast< size_type >( this->__end_cap() - this->__begin );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
vector< _Tp, _Allocator >::shrink_to_fit() LLVM_MSTL_NOEXCEPT {
	if ( capacity() > size() ) {
		allocator_type&                               __a = this->__alloc();
		__split_buffer< value_type, allocator_type& > __v( size(), size(), __a );
		__swap_out_circular_buffer( __v );
	}
}

/*************************************************************************************		
 *                                                                                   *
 *															 		CAPACITY END			               	               *
 *                                                                                   *
 *************************************************************************************/

/*************************************************************************************		
 *                                                                                   *
 *															 	MODIFIERS BEGIN			               	               *
 *                                                                                   *
 *************************************************************************************/

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
vector< _Tp, _Allocator >::clear() LLVM_MSTL_NOEXCEPT {
	size_type __old_size = size();
	__clear();
	__annotate_shrink( __old_size );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
vector< _Tp, _Allocator >::insert( const_iterator __position, const_reference __x )
	-> typename vector< _Tp, _Allocator >::iterator {
	pointer __p = this->__begin + ( __position - begin() );
	if ( !core::is_constant_evaluated() && this->__end < this->__end_cap() ) {
		if ( __p == this->__end ) {
			__construct_one_at_end( (size_t) __x );
		} else {
			__move_range( __p, this->__end, __p + 1 );
			const_pointer __xr = core::pointer_traits< const_pointer >::pointer_to( __x );
			if ( __p <= __xr && __xr < this->__end )
				++__xr;
			*__p = *__xr;
		}
	} else {
		allocator_type&                               __a = this->__alloc();
		__split_buffer< value_type, allocator_type& > __v(
			__recommend( size() + 1 ), (size_t) ( __p - this->__begin ), __a );
		__v.push_back( __x );
		__p = __swap_out_circular_buffer( __v, __p );
	}
	return __make_iter( __p );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
vector< _Tp, _Allocator >::insert( const_iterator __position, value_type&& __x )
	-> typename vector< _Tp, _Allocator >::iterator {
	pointer __p = this->__begin + ( __position - begin() );
	if ( this->__end < this->__end_cap() ) {
		if ( __p == this->__end ) {
			__construct_one_at_end( static_cast< size_type >( core::move( __x ) ) );
		} else {
			__move_range( __p, this->__end, __p + 1 );
			*__p = core::move( __x );
		}
	} else {
		allocator_type&                               __a = this->__alloc();
		__split_buffer< value_type, allocator_type& > __v( __recommend( size() + 1 ), (size_t) ( __p - this->__begin ), __a );
		__v.push_back( core::move( __x ) );
		__p = __swap_out_circular_buffer( __v, __p );
	}
	return __make_iter( __p );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
vector< _Tp, _Allocator >::insert( const_iterator __position, size_type __n, const_reference __x )
	-> typename vector< _Tp, _Allocator >::iterator {
	pointer __p = this->__begin + ( __position - begin() );
	if ( __n > 0 ) {
		if ( !core::is_constant_evaluated() && __n <= static_cast< size_type >( this->__end_cap() - this->__end ) ) {
			size_type __old_n    = __n;
			pointer   __old_last = this->__end;
			if ( __n > static_cast< size_type >( this->__end - __p ) ) {
				size_type __cx = __n - static_cast< size_type >( this->__end - __p );
				__construct_at_end( __cx, __x );
				__n -= __cx;
			}
			if ( __n > 0 ) {
				__move_range( __p, __old_last, __p + __old_n );
				const_pointer __xr = core::pointer_traits< const_pointer >::pointer_to( __x );
				if ( __p <= __xr && __xr < this->__end ) {
					__xr += __old_n;
					core::fill_n( __p, __n, *__xr );
				}
			}
		} else {
			allocator_type&                               __a = this->__alloc();
			__split_buffer< value_type, allocator_type& > __v(
				__recommend( size() + __n ), static_cast< size_type >( __p - this->__begin ), __a );
			__v.__construct_at_end( __n, __x );
			__p = __swap_out_circular_buffer( __v, __p );
		}
	}
	return __make_iter( __p );
}

template < class _Tp, class _Allocator >
template <
	class _InputIterator,
	core::enable_if_t<
		__is_exactly_cpp17_input_iterator< _InputIterator >::value &&
			core::is_constructible_v<
				_Tp,
				typename core::iterator_traits< _InputIterator >::reference >,
		int > >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
vector< _Tp, _Allocator >::insert( const_iterator __position, _InputIterator __first, _InputIterator __last )
	-> typename vector< _Tp, _Allocator >::iterator {
	difference_type __off      = __position - begin();
	pointer         __p        = this->__begin + __off;
	allocator_type& __a        = this->__alloc();
	pointer         __old_last = this->__end;
	for ( ; this->__end != this->__end_cap() && __first != __last; ++__first ) {
		__construct_one_at_end( *__first );
	}
	__split_buffer< value_type, allocator_type& > __v( __a );
	if ( __first != __last ) {
		__v.__construct_at_end( __first, __last );
		difference_type __old_size = __old_last - this->__begin;
		difference_type __old_p    = __p - this->__begin;
		reserve( __recommend( size() + __v.size() ) );
		__p        = this->__begin + __old_p;
		__old_last = this->__begin + __old_size;
	}
	__p = core::rotate( __p, __old_last, this->__end );
	insert( __make_iter( __p ), core::make_move_iterator( __v.begin() ), core::make_move_iterator( __v.end() ) );
	return begin() + __off;
}

template < class _Tp, class _Allocator >
template <
	class _ForwardIterator,
	core::enable_if_t<
		__is_cpp17_forward_iterator< _ForwardIterator >::value &&
			core::is_constructible_v<
				_Tp,
				typename core::iterator_traits< _ForwardIterator >::reference >,
		int > >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
vector< _Tp, _Allocator >::insert( const_iterator __position, _ForwardIterator __first, _ForwardIterator __last )
	-> typename vector< _Tp, _Allocator >::iterator {
	pointer         __p = this->__begin + ( __position - begin() );
	difference_type __n = core::distance( __first, __last );

	if ( __n > 0 ) {
		if ( __n <= this->__end_cap() - this->__end ) {
			size_type        __old_n    = static_cast< size_type >( __n );
			pointer          __old_last = this->__end;
			_ForwardIterator __m        = __last;
			difference_type  __dx       = this->__end - __p;
			if ( __n > __dx ) {
				__m                    = __first;
				difference_type __diff = this->__end - __p;
				core::advance( __m, __diff );
				__construct_at_end( __m, __last, static_cast< size_type >( __n - __diff ) );
				__n = __dx;
			}
			if ( __n > 0 ) {
				__move_range( __p, __old_last, __p + __old_n );
				core::copy( __first, __m, __p );
			}
		} else {
			allocator_type&                               __a = this->__alloc();
			__split_buffer< value_type, allocator_type& > __v(
				__recommend( size() + (size_type) __n ), static_cast< size_type >( __p - this->__begin ), __a );
			__v.__construct_at_end( __first, __last );
			__p = __swap_out_circular_buffer( __v, __p );
		}
	}

	return __make_iter( __p );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
vector< _Tp, _Allocator >::insert( const_iterator __position, core::initializer_list< value_type > __il )
	-> typename vector< _Tp, _Allocator >::iterator {
	return insert( __position, __il.begin(), __il.end() );
}

/*************************************************************************************		
 *                                                                                   *
 *															  	MODIFIERS END			               	               *
 *                                                                                   *
 *************************************************************************************/

/*************************************************************************************		
 *                                                                                   *
 *																 HELPER BEGIN 			                               *
 *                                                                                   *
 *************************************************************************************/

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto vector< _Tp, _Allocator >::__vallocate( size_type __n ) -> void {
	if ( __n > max_size() ) {
		__throw_length_error();
	}
	/**
	* @brief allocator_at_least tries to allocate a storage for at least n value_type objects, 
	* and provides a fallback mechanism that allocates a storage for exact n objects.
	*
	* @param ptr a pointer of type Pointer which is typically used for the `address of the first element` 
	* in the storage allocated by allocate_at_least
	* @param count a value of type SizeType which is typically used for the `actual number of elements` 
	* in the storage allocated by allocate_at_least
	* 
	* @return return the `allocation_result<class Pointer, class SizeType = std::size_t>`
	*/
	auto __allocation = __allocate_at_least( __alloc(), __n );
	__begin           = __allocation.ptr;            //<--- causes `__begin` to point to the address of the memory allocated by the allocator
	__end             = __allocation.ptr;            //<--- causes' __end 'to point to the address of the memory allocated by the allocator
	__end_cap()       = __begin + __allocation.count;//<--- '__end_capm.first' holds the reference to '__end',
																									 //<--- so here '__end_cap()' is modified to point to the last position of the element's last position

	//! here `__annotate_new` doing nothing
	__annotate_new( 0 );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto vector< _Tp, _Allocator >::__vdeallocate() LLVM_MSTL_NOEXCEPT {
	if ( this->__begin != nullptr ) {
		clear();
		__alloc_traits::deallocate( this->__alloc(), this->__begin, capacity() );
		this->__begin = this->__end = this->__end_cap() = nullptr;
	}
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 LLVM_MSTL_TEMPLATE_INLINE auto vector< _Tp, _Allocator >::__recommend( size_type __new_size ) const
	-> typename vector< _Tp, _Allocator >::size_type {
	const size_type __ms = max_size();
	if ( __new_size > __ms ) this->__throw_length_error();
	const size_type __cap = capacity();
	if ( __cap >= __ms / 2 ) return __ms;
	return core::max< size_type >( 2 * __cap, __new_size );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto vector< _Tp, _Allocator >::__swap_out_circular_buffer( __split_buffer< value_type, allocator_type& >& __v ) {
	__annotate_delete();//!<--- no work
	using _RevIter = core::reverse_iterator< pointer >;
	__v.__begin    = __uninitialized_allocator_move_if_noexcept(
                  __alloc(),
                  _RevIter( __end ),
                  _RevIter( __begin ),
                  _RevIter( __v.__begin ) )
									.base();
	core::swap( this->__begin, __v.__begin );
	core::swap( this->__end, __v.__end );
	core::swap( this->__end_cap(), __v.__end_cap() );
	__v.__first = __v.__begin;
	__annotate_new( size() );//!<--- no work
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto vector< _Tp, _Allocator >::__swap_out_circular_buffer(
	__split_buffer< value_type, allocator_type& >& __v, pointer __p ) -> typename vector< _Tp, _Allocator >::pointer {
	__annotate_delete();
	pointer __r    = __v.__begin;
	using _RevIter = core::reverse_iterator< pointer >;
	__v.__begin    = __uninitialized_allocator_move_if_noexcept(
                  __alloc(),
                  _RevIter( __p ),
                  _RevIter( __begin ),
                  _RevIter( __v.__begin ) )
									.base();
	__v.__end = __uninitialized_allocator_move_if_noexcept( __alloc(), __p, __end, __v.__end );
	core::swap( this->__begin, __v.__begin );
	core::swap( this->__end, __v.__end );
	core::swap( this->__end_cap(), __v.__end_cap() );
	__v.__first = __v.__begin;
	__annotate_new( size() );
	return __r;
}


template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto vector< _Tp, _Allocator >::__construct_at_end( size_type __n ) {
	_ConstructTransaction __tx( *this, __n );
	const_pointer         __new_end = __tx.__new_end;//<--- get the really pointer, which point the `end position`
	for ( pointer __pos = __tx.__pos; __pos != __new_end; __tx.__pos = ++__pos ) {
		//<--- `std::to_address` to gain the origin pointer which obtained the pointer or smart pointer
		//<--- `static constexpr void construct( Alloc& a, T* p, Args&&... args )`
		//<--- `p` pointer to the uninitialized storage on which a T object will be constructed
		//!<--- so, here is actually constructed `pointer` by `pointer`
		__alloc_traits::construct( this->__alloc(), core::to_address( __pos ) );
	}
	//! When the `__tx` drop the block, it will call `__tx.~_ConstructTransaction`.
	//! Then, execute this statement `__v.__end = __pos` to update the right `__end` point to the right position
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_INLINE
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
	vector< _Tp, _Allocator >::__construct_at_end( size_type __n, const_reference __x ) {
	_ConstructTransaction __tx( *this, __n );
	const_pointer         __new_end = __tx.__new_end;
	for ( pointer __pos = __tx.__pos; __pos != __new_end; __tx.__pos = ++__pos ) {
		//<--- this `construct` use the `__x` to initialize when alloc the `__pos`
		__alloc_traits::construct( this->__alloc(), core::to_address( __pos ), __x );
	}
	//! When the `__tx` drop the block, it will call `__tx.~_ConstructTransaction`.
	//! Then, execute this statement `__v.__end = __pos` to update the right `__end` point to the right position
}

template < typename _Tp, typename _Allocator >
template <
	typename _ForwardIterator,
	core::enable_if_t<
		__is_cpp17_forward_iterator< _ForwardIterator >::value, int > >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
vector< _Tp, _Allocator >::__construct_at_end( _ForwardIterator __first, _ForwardIterator __last, size_type __n ) {
	_ConstructTransaction __tx( *this, __n );
	__tx.__pos = __uninitialized_allocator_copy( __alloc(), __first, __last, __tx.__pos );
}

template < typename _Tp, typename _Allocator >
template < typename... _Args >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto vector< _Tp, _Allocator >::__emplace_back_slow_path( _Args&&... __args ) {
	allocator_type&                               __a = this->__alloc();
	__split_buffer< value_type, allocator_type& > __v( __recommend( size() + 1 ), size(), __a );
	__alloc_traits::construct( __a, core::to_address( __v.__end ), std::forward< _Args >( __args )... );
	__v.__end++;
	__swap_out_circular_buffer( __v );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
vector< _Tp, _Allocator >::__move_range( pointer __from_s, pointer __from_e, pointer __to ) {
	pointer         __old_last = this->__end;
	difference_type __n        = __old_last - __to;
	{
		pointer               __i = __from_s + __n;
		_ConstructTransaction __tx( *this, static_cast< size_type >( __from_e - __i ) );
		for ( pointer __pos = __tx.__pos; __i < __from_e; ++__i, (void) ++__pos, __tx.__pos = __pos ) {
			__alloc_traits::construct( this->__alloc(), core::to_address( __pos ), core::move( *__i ) );
		}
	}
	core::move_backward( __from_s, __from_s + __n, __old_last );
}

template < typename _Tp, typename _Allocator >
template < typename... _Args >
LLVM_MSTL_TEMPLATE_INLINE
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto
	vector< _Tp, _Allocator >::emplace_back( _Args&&... __args )
		-> typename vector< _Tp, _Allocator >::reference {
	if ( this->__end < this->__end_cap() ) {
		__construct_one_at_end( std::forward< _Args >( __args )... );
	} else {
		__emplace_back_slow_path( std::forward< _Args >( __args )... );
	}
	return this->back();
}

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_VECTOR_H
