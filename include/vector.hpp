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
#include "__iterator/wrap_iter.h"
#include "__memory/compress_pair.h"
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
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 explicit vector( size_type __n, const allocator_type& __a );
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector( size_type __n, const value_type& __x );
	template < typename = core::enable_if_t< __is_allocator< _Allocator >::value > >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	vector( size_type __n, const value_type& __x, const allocator_type& __a )
	    : __end_capm( nullptr, __a ) {
		if ( __n > 0 ) {
			__vallocate( __n );
			__construct_at_end( __n, __x );
		}
	}

private:
	class __destroy_vector {
	public:
		LLVM_MSTL_CONSTEXPR __destroy_vector( vector& __vec )
		    : __vec( __vec ) {}

		LLVM_MSTL_CONSTEXPR_SINCE_CXX20 void operator()() {
			__vec.__annotate_delete();
			if ( __vec.__begin != nullptr ) {
				__vec.__clear();
				__alloc_traits::deallocate( __vec.__alloc(), __vec.__begin, __vec.capacity() );
			}
		}

	private:
		vector __vec;
	};

public:
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto size() const LLVM_MSTL_NOEXCEPT->size_type {
		return static_cast< size_type >( this->__end - this->__begin );
	}
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto capacity() const LLVM_MSTL_NOEXCEPT->size_type* {
		return static_cast< size_type >( __end_cap() - this->__begin );
	}
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto data() LLVM_MSTL_NOEXCEPT->value_type* {
		return core::to_address( this->__begin );
	}

private:
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

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __construct_at_end( size_type __n );
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __construct_at_end( size_type __n, const_reference __x );

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __annotate_contiguous_container( const void*, const void*, const void*, const void* ) const LLVM_MSTL_NOEXCEPT {}

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __annotate_new( size_type __current_size ) const LLVM_MSTL_NOEXCEPT {
		__annotate_contiguous_container( data(), data() + capacity(), data() + capacity(), data() + __current_size );
	}

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __annotate_delete() const LLVM_MSTL_NOEXCEPT {
		__annotate_contiguous_container( data(), data() + capacity(), data() + size(), data() + capacity() );
	}

	struct _ConstructTransaction {
		LLVM_MSTL_CONSTEXPR_SINCE_CXX20 explicit _ConstructTransaction( vector& __v, size_type __n )
		    : __v( __v )
		    , __pos( __v.__end )
		    , __new_end( __v.__end + __n ) {}

		LLVM_MSTL_CONSTEXPR_SINCE_CXX20 ~_ConstructTransaction() {
			__v.__end = __pos;
		}

		vector&             __v;
		pointer             __pos;
		const_pointer const __new_end;
	};

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

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __clear() LLVM_MSTL_NOEXCEPT {
		__base_destruct_at_end( this->__begin );
	}

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __base_destruct_at_end( pointer __new_last ) LLVM_MSTL_NOEXCEPT {
		pointer __soon_to_be_end = this->__end;
		while ( __new_last != __soon_to_be_end ) {
			__alloc_traits::destroy( __alloc(), core::to_address( --__soon_to_be_end ) );
			this->__end = __new_last;
		}
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
		__vallocate( __n );//<--- allocate a memmory and set `__begin`、`__end` and `__end_capm`
		__construct_at_end( __n );
	}
	__guard.__complete();
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto vector< _Tp, _Allocator >::__construct_at_end( size_type __n ) {
	_ConstructTransaction __tx( *this, __n );
	const_pointer         __new_end = __tx.__new_end;
	for ( pointer __pos = __tx.__pos; __pos != __new_end; __tx.__pos = ++__pos ) {
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
		__alloc_traits::construct( this->__alloc(), core::to_address( __pos ), __x );
	}
}

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_VECTOR_H
