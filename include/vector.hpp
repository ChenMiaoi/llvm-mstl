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
#include "__memory/compress_pair.h"
#include "__split_buffer.h"
#include "__type_traits/is_allocator.h"
#include "__utility/exception_guard.h"
#include "stdexcept.h"


#include <iterator>
#include <memory>
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

	/**
		* @brief Default constructor for creating an empty vector.
		*
		* This constructor creates an empty vector object without any parameters.
		* It is available since C++20 and can be evaluated as a constant expression.
		* The constructor is declared as noexcept if the allocator_type's default constructor
		* is marked as noexcept.
		*
		* @tparam allocator_type The type of the allocator used by the vector.
		*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector()
	  LLVM_MSTL_NOEXCEPT_V( core::is_nothrow_default_constructible_v< allocator_type > );

	/**
		* @brief Explicit constructor for creating a vector with a specified allocator.
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
		* This constructor initializes a vector with a specified size, creating `__n` elements.
		* It allocates memory using the specified allocator, constructs elements at the end of the vector,
		* and completes the construction process.
		*
		* @tparam _Tp The type of elements stored in the vector.
		* @tparam _Allocator The allocator type used for memory allocation.
		* @param __n The size of the vector.
		*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 explicit vector( size_type __n );

	/**
		* @brief Constructs a vector with a specified size and allocator.
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
		vector __vec;
	};

public:
	/**
	* @brief Returns the size of the vector.
	*
	* This member function returns the number of elements in the vector by subtracting the pointer `__begin`
	* from the pointer `__end` and casting the result to the size type `size_type`.
	*
	* @return The number of elements in the vector.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto size() const LLVM_MSTL_NOEXCEPT->size_type {
		return static_cast< size_type >( this->__end - this->__begin );
	}
	/**
	* @brief Returns the capacity of the vector.
	*
	* This member function returns the capacity of the vector, which represents the maximum number of elements
	* that the vector can hold without reallocating memory. The capacity is calculated by subtracting the pointer
	* returned by `__end_cap()` from the pointer `__begin` and casting the result to the size type `size_type`.
	*
	* @return The capacity of the vector.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto capacity() const LLVM_MSTL_NOEXCEPT->size_type* {
		return static_cast< size_type >( __end_cap() - this->__begin );
	}
	/**
	* @brief Returns a pointer to the underlying data of the vector.
	*
	* This member function returns a pointer to the underlying data of the vector. The pointer is obtained
	* by using the `to_address` function from the C++20 standard library. It converts the iterator `__begin`
	* to a pointer of type `value_type*`.
	*
	* @return A pointer to the underlying data of the vector.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto data() LLVM_MSTL_NOEXCEPT->value_type* {
		return core::to_address( this->__begin );
	}

	template < typename... _Args >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto emplace_back( _Args&&... __args ) -> reference;

private:
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
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __vallocate( size_type __n ) -> void {
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
		auto __allocation = core::allocator_traits< allocator_type >::allocate_at_least( __alloc(), __n );
		__begin           = __allocation.ptr;            //<--- causes `__begin` to point to the address of the memory allocated by the allocator
		__end             = __allocation.ptr;            //<--- causes' __end 'to point to the address of the memory allocated by the allocator
		__end_cap()       = __begin + __allocation.count;//<--- '__end_capm.first' holds the reference to '__end',
		                                                 //<--- so here '__end_cap()' is modified to point to the last position of the element's last position

		//! here `__annotate_new` doing nothing
		__annotate_new( 0 );
	}

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

	template < typename... _Args >
	LLVM_MSTL_TEMPLATE_INLINE LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __emplace_back_slow_path( _Args&&... __args );

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __annotate_contiguous_container( const void*, const void*, const void*, const void* ) const LLVM_MSTL_NOEXCEPT {}

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __annotate_new( size_type __current_size ) const LLVM_MSTL_NOEXCEPT {
		__annotate_contiguous_container( data(), data() + capacity(), data() + capacity(), data() + __current_size );
	}

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __annotate_delete() const LLVM_MSTL_NOEXCEPT {
		__annotate_contiguous_container( data(), data() + capacity(), data() + size(), data() + capacity() );
	}

	/**
		* @brief A transaction for reserving space at the end of a vector.
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

	LLVM_MSTL_NORETURN auto __throw_length_error() const {
		nya::__throw_length_error( "vector" );
	}

public:
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto max_size() const LLVM_MSTL_NOEXCEPT->size_type;

private:
	pointer                                      __begin    = nullptr;//<--- point the `first` element in vector
	pointer                                      __end      = nullptr;//<--- point the last element in vector(not really `last`, `the last afther the last one`)
	__compressed_pair< pointer, allocator_type > __end_capm =         //<--- use less memory to store the end point and allocator
	                                                                  //<--- the `cap.first` is the `end`, point the `last element afther the last one`
	                                                                  //<--- the `cap.second` is the `allocator`, use to manage memory's alloc and release
	  __compressed_pair< pointer, allocator_type >( nullptr, __default_init_tag() );
};

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector< _Tp, _Allocator >::vector( size_type __n ) {
	// create a RAII exception guard, if task done, automatic drop by calling `__destroy_vector`
	// use `*this` because we don't want to drop the `this`, just want a copy vec
	auto __guard = __make_exception_guard( __destroy_vector( *this ) );
	if ( __n > 0 ) {
		__vallocate( __n );       //<--- allocate a memmory and set `__begin`、`__end` and `__end_capm`.
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
