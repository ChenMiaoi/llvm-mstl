#ifndef LLVM_MSTL_SPLIT_BUFFER_H
#define LLVM_MSTL_SPLIT_BUFFER_H

#include "__config.h"
#include "__iterator/iterator_traits.h"
#include "__memory/allocate_at_least.h"
#include "__memory/compress_pair.h"
#include "__memory/swap_allocator.h"


#include <algorithm>
#include <iterator>
#include <memory>
#include <type_traits>

LLVM_MSTL_CORE_STD

LLVM_MSTL_BEGIN_NAMESPACE_STD

/**
 * @brief __split_buffer allocates a contiguous chunk of memory and stores objects in the range [__begin_, __end_).
 * 
 * The __split_buffer struct represents a buffer that allocates a contiguous chunk of memory and stores objects in the range [__begin_, __end_).
 * It has uninitialized memory in the ranges [__first_, __begin_) and [__end_, __end_cap_.first()).
 * This allows it to grow both in the front and back without having to move the existing data.
 * 
 * @tparam _Tp The type of objects stored in the buffer.
 * @tparam _Allocator The allocator type used for memory management. Defaults to core::allocator<_Tp>.
 */
template < typename _Tp, typename _Allocator = core::allocator< _Tp > >
struct __split_buffer {
private:
	__split_buffer( const __split_buffer& );
	__split_buffer& operator=( const __split_buffer& );

public:
	using value_type      = _Tp;
	using allocator_type  = _Allocator;
	using __alloc_rr      = core::remove_reference_t< allocator_type >;
	using __alloc_traits  = core::allocator_traits< __alloc_rr >;
	using reference       = value_type&;
	using const_reference = const value_type&;
	using size_type       = typename __alloc_traits::size_type;
	using difference_type = typename __alloc_traits::difference_type;
	using pointer         = typename __alloc_traits::pointer;
	using const_pointer   = typename __alloc_traits::const_pointer;
	using iterator        = pointer;
	using const_iterator  = const_pointer;

	pointer                                      __first;   //<--- Pointer to the first element in the buffer.
	pointer                                      __begin;   //<--- Pointer to the beginning of the constructed range.
	pointer                                      __end;     //<--- Pointer to the end of the constructed range.
	__compressed_pair< pointer, allocator_type > __end_capm;//<--- '__end_capm.first' holds the reference to '__end',
																													//<--- so here '__end_cap()' is modified to point to the last position of the element's last position

	using __alloc_ref       = core::add_lvalue_reference_t< allocator_type >;
	using __alloc_const_ref = core::add_lvalue_reference_t< allocator_type >;

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __alloc() LLVM_MSTL_NOEXCEPT->__alloc_rr& {
		return __end_capm.second();
	}
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __alloc() const LLVM_MSTL_NOEXCEPT->const __alloc_rr& {
		return __end_capm.second();
	}
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __end_cap() LLVM_MSTL_NOEXCEPT->pointer& {
		return __end_capm.first();
	}
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __end_cap() const LLVM_MSTL_NOEXCEPT->const pointer& {
		return __end_capm.first();
	}

	/*********************************		Constructor		*********************************************/

	/**
  * @brief Default constructor for the __split_buffer.
  *
  * This constructor initializes the __split_buffer object. It sets the member variables to default values.
  *
  * @tparam _Tp The type of objects stored in the buffer.
  * @tparam _Allocator The allocator type used for memory management.
  * @throws None
  */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 __split_buffer()
		LLVM_MSTL_NOEXCEPT_V( core::is_nothrow_default_constructible_v< allocator_type > );
	/**
  * @brief Constructor for the __split_buffer with an allocator.
  *
  * This constructor initializes the __split_buffer object with a specified allocator.
  *
  * @tparam _Tp The type of objects stored in the buffer.
  * @tparam _Allocator The allocator type used for memory management.
  * @param __a The allocator object.
  * @throws None
  */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 explicit __split_buffer( __alloc_rr& __a );
	/**
  * @brief Constructor for the __split_buffer with a const reference to the allocator.
  *
  * This constructor initializes the __split_buffer object with a specified allocator, taking a const reference to the allocator.
  *
  * @tparam _Tp The type of objects stored in the buffer.
  * @tparam _Allocator The allocator type used for memory management.
  * @param __a The const reference to the allocator object.
  * @throws None
  */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 explicit __split_buffer( const __alloc_rr& __a );
	/**
  * @brief Constructor for the __split_buffer with capacity, starting position, and an allocator.
  *
  * This constructor initializes the __split_buffer object with a specified capacity, starting position, and allocator.
  *
  * @tparam _Tp The type of objects stored in the buffer.
  * @tparam _Allocator The allocator type used for memory management.
  * @param __cap The capacity of the buffer.
  * @param __start The starting position in the buffer.
  * @param __a The allocator object.
  * @throws None
  */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 __split_buffer( size_type __cap, size_type __start, __alloc_rr& __a );
	/**
	* @brief Move constructor for __split_buffer.
	*
	* This constructor initializes a new __split_buffer by moving the contents of another __split_buffer.
	*
	* @tparam _Tp The element type.
	* @tparam _Allocator The allocator type.
	* @param __c The __split_buffer to be moved.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 __split_buffer( __split_buffer&& __c )
		LLVM_MSTL_NOEXCEPT_V( core::is_nothrow_move_constructible_v< allocator_type > );
	/**
	* @brief Move constructor for __split_buffer.
	* @tparam _Tp The type of elements in the split buffer.
	* @tparam _Allocator The allocator type used for memory management.
	* @param __c The source __split_buffer to move from.
	* @param __a The allocator to use for the new __split_buffer.
	*
	* This move constructor initializes a new __split_buffer by moving the contents from the source __split_buffer, __c.
	* If the allocator __a is the same as the allocator of __c, the ownership of the memory is transferred directly.
	* Otherwise, a new memory allocation is performed using the allocator __a, and the elements are move-constructed into the new memory.
	* After the move, the source __split_buffer __c is left in a valid but unspecified state.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 __split_buffer( __split_buffer&& __c, const __alloc_rr& __a );
	/**
  * @brief Destructor for the __split_buffer.
  *
  * This destructor cleans up the __split_buffer object by clearing its contents and deallocating the memory if necessary.
  *
  * @tparam _Tp The type of objects stored in the buffer.
  * @tparam _Allocator The allocator type used for memory management.
  * @throws None
  */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 ~__split_buffer();

	/************************************		Operators		**************************************/

	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 __split_buffer& operator=( __split_buffer&& __c )
		LLVM_MSTL_NOEXCEPT_V( (__alloc_traits::propagate_on_container_move_assignment::value &&
													 core::is_nothrow_move_assignable_v< allocator_type >) ||
													!__alloc_traits::propagate_on_container_move_assignment::value );

	/************************************		Iterators		**************************************/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 iterator       begin() LLVM_MSTL_NOEXCEPT { return __begin; }
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 const_iterator begin() const LLVM_MSTL_NOEXCEPT { return __begin; }
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 iterator       end() LLVM_MSTL_NOEXCEPT { return __end; }
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 const_iterator end() const LLVM_MSTL_NOEXCEPT { return __end; }

	/************************************		Functions		**************************************/
	/**
	* TODO
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto size() const -> size_type { return static_cast< size_type >( __end - __begin ); }
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto clear() LLVM_MSTL_NOEXCEPT { __destruct_at_begin( __begin ); };
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto capacity() const -> size_type const { return static_cast< size_type >( __end_cap() - __first ); };
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __front_spare() const -> size_type { return static_cast< size_type >( __begin - __first ); }
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __back_spare() const -> size_type { return static_cast< size_type >( __end_cap() - __end ); }

	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto front() -> reference { return *__begin; }
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto front() const -> const_reference { return *__begin; }
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto back() -> reference { return *( __end - 1 ); }
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto back() const -> const_reference { return *( __end - 1 ); }

	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto reserve( size_type __n );
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto shrink_to_fit() LLVM_MSTL_NOEXCEPT;
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto push_front( const_reference __x );
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto push_front( value_type&& __x );
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto push_back( const_reference __x );
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto push_back( value_type&& __x );
	/**
	 * TODO
	 */
	template < typename... _Args >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto emplace_back( _Args&&... _args );
	/**
	 * TODO 
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto pop_front() { __destruct_at_begin( __begin + 1 ); }
	/**
	 * TODO 
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto pop_back() { __destruct_at_end( __end - 1 ); }
	/**
	* @brief Swaps the contents and allocators of two split buffer objects.
	* @tparam _Tp The type of elements in the split buffer.
	* @tparam _Allocator The allocator type used for memory management.
	* @param __x The split buffer object to swap with.
	*
	* This function swaps the contents and allocators of the current split buffer object with another split buffer object, __x.
	* The swap operation includes swapping the __first, __begin, __end, and __end_cap() members.
	* If the propagate_on_container_swap trait of the allocator type is true, the allocators are swapped using __swap_allocator.
	* The function is noexcept if either the propagate_on_container_swap trait is false or the allocator type is nothrow swappable.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto swap( __split_buffer& __x )
		LLVM_MSTL_NOEXCEPT_V(
			!__alloc_traits::propagate_on_container_swap::value ||
			core::is_nothrow_swappable_v< __alloc_rr > );

	/********************************		Help Constructor		**************************************/
	/**
	* @brief Constructs and initializes elements at the end of the split buffer.
	* @tparam _Tp The type of elements in the split buffer.
	* @tparam _Allocator The allocator type used for memory management.
	* @param __n The number of elements to construct.
	*
	* This function constructs and initializes elements at the end of the split buffer.
	* It uses the _ConstructTransaction object to manage the construction process.
	* The __n parameter determines the number of elements to be constructed.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __construct_at_end( size_type __n );
	/**
	* @brief Constructs and initializes elements at the end of the split buffer with a specified value.
	* @tparam _Tp The type of elements in the split buffer.
	* @tparam _Allocator The allocator type used for memory management.
	* @param __n The number of elements to construct.
	* @param __x The value to initialize the elements with.
	*
	* This function constructs and initializes elements at the end of the split buffer with the specified value.
	* It uses the _ConstructTransaction object to manage the construction process.
	* The __n parameter determines the number of elements to be constructed.
	* The __x parameter is the value used to initialize the elements.
	*/
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __construct_at_end( size_type __n, const_reference __x );
	/**
	* @brief Constructs and initializes elements at the end of the split buffer from a range of input iterators.
	* @tparam _Tp The type of elements in the split buffer.
	* @tparam _Allocator The allocator type used for memory management.
	* @tparam _InputIter The type of input iterators.
	* @param __fst The beginning of the input iterator range.
	* @param __last The end of the input iterator range.
	*
	* This function constructs and initializes elements at the end of the split buffer from a range of input iterators.
	* It is only enabled if the _InputIter type satisfies the requirements of exactly C++17 input iterator.
	* The elements are constructed using the allocator __a.
	* If the split buffer reaches its capacity during construction, a new split buffer with increased capacity is created and the elements are moved to the new buffer using move construction.
	* The old split buffer is swapped with the new buffer to ensure proper memory management.
	* Finally, the remaining elements from the input iterator range are constructed at the end of the split buffer.
	*
	* @code{.cc}
	* // Create a split buffer of integers with a custom allocator
  *  __split_buffer<int, std::allocator<int>> buffer;
	*
  *  // Populate the split buffer with elements from a vector
  *  nya::vector<int> vec = {1, 2, 3, 4, 5};
  *  buffer.__construct_at_end(vec.begin(), vec.end());
	* @endcode
	* 
	*/
	template < typename _InputIter >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __construct_at_end( _InputIter __first, _InputIter __last )
		-> core::enable_if_t< __is_exactly_cpp17_input_iterator< _InputIter >::value >;
	/**
	* @brief Constructs and initializes elements at the end of the split buffer from a range of forward iterators.
	* @tparam _Tp The type of elements in the split buffer.
	* @tparam _Allocator The allocator type used for memory management.
	* @tparam _ForwardIter The type of forward iterators.
	* @param __fst The beginning of the forward iterator range.
	* @param __last The end of the forward iterator range.
	*
	* This function constructs and initializes elements at the end of the split buffer from a range of forward iterators.
	* It is only enabled if the _ForwardIter type satisfies the requirements of C++17 forward iterator.
	* The elements are constructed using the allocator and stored in the split buffer.
	* The function uses _ConstructTransaction to track the construction progress and the number of elements to construct.
	* For each element in the iterator range, the allocator constructs the element at the position indicated by __pos.
	* The __pos is incremented to point to the next position in the split buffer, and __fst is advanced to the next element in the iterator range.
	*
	*	@code{.cc}
	* // Create a split buffer of integers with a custom allocator
  *  __split_buffer<int, std::allocator<int>> buffer;
	*
  *  // Populate the split buffer with elements from a vector
  *  nya::vector<int> vec = {1, 2, 3, 4, 5};
  *  buffer.__construct_at_end(vec.begin(), vec.end());
	* @endcode
	*
	*/
	template < typename _ForwardIter >
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __construct_at_end( _ForwardIter __first, _ForwardIter __last )
		-> core::enable_if_t< __is_cpp17_forward_iterator< _ForwardIter >::value >;

	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __destruct_at_begin( pointer __new_begin ) {
		__destruct_at_begin( __new_begin, core::is_trivially_destructible< value_type >() );
	};
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __destruct_at_begin( pointer __new_begin, core::false_type );
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __destruct_at_begin( pointer __new_begin, core::true_type );
	/**
	 * TODO 
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __destruct_at_end( pointer __new_last ) LLVM_MSTL_NOEXCEPT {
		__destruct_at_end( __new_last, core::false_type() );
	};
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __destruct_at_end( pointer __new_last, core::false_type ) LLVM_MSTL_NOEXCEPT;
	/**
	 * TODO 
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __destruct_at_end( pointer __new_last, core::true_type ) LLVM_MSTL_NOEXCEPT;
	/**
	 * TODO 
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __invariants() const -> bool;

private:
	/**
	 * TODO
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __move_assign_alloc( __split_buffer& __c, core::true_type )
		LLVM_MSTL_NOEXCEPT_V( core::is_nothrow_move_assignable_v< allocator_type > ) {
		__alloc() = core::move( __c.__alloc() );
	}

	/**
	 * TODO 
	 */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __move_assign_alloc( __split_buffer&, core::false_type )
		LLVM_MSTL_NOEXCEPT {}

	/********************************		Help Structor 	**********************************/
	/**
	* @struct _ConstructTransaction
	* @brief Represents a transaction used for constructing an object.
	*
	* This struct is used to manage the construction of an object by tracking the position, end, and destination pointers.
	*/
	struct _ConstructTransaction {
		/**
     * @brief Constructs a _ConstructTransaction object.
     * @param __p Pointer to the starting position of the object.
     * @param __n The size of the object.
     *
     * This constructor initializes the __pos, __end, and __dest members based on the provided parameters.
     */
		LLVM_MSTL_CONSTEXPR_SINCE_CXX20 explicit _ConstructTransaction( pointer* __p, size_type __n )
			LLVM_MSTL_NOEXCEPT
				: __pos( *__p ),
					__end( *__p + __n ),
					__dest( __p ){};

		/**
     * @brief Destroys a _ConstructTransaction object.
     *
     * This destructor assigns the value of __pos to *__dest.
     */
		LLVM_MSTL_CONSTEXPR_SINCE_CXX20 ~_ConstructTransaction() {
			*__dest = __pos;
		}

	public:
		pointer       __pos;//<--- The current position pointer.
		const pointer __end;//<--- The end position pointer.

	private:
		pointer* __dest;//<--- The destination pointer.
	};
};

/*********************************		Constructor		*********************************************/
template < typename _Tp, typename _Allocator >
LLVM_MSTL_TEMPLATE_INLINE
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	__split_buffer< _Tp, _Allocator >::__split_buffer() LLVM_MSTL_NOEXCEPT_V( core::is_nothrow_default_constructible_v< allocator_type > )
		: __first( nullptr )
		, __begin( nullptr )
		, __end( nullptr )
		, __end_capm( nullptr, __default_init_tag() ) {}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_TEMPLATE_INLINE
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	__split_buffer< _Tp, _Allocator >::__split_buffer( __alloc_rr& __a )
		: __first( nullptr )
		, __begin( nullptr )
		, __end( nullptr )
		, __end_capm( nullptr, __a ) {}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_TEMPLATE_INLINE
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	__split_buffer< _Tp, _Allocator >::__split_buffer( const __alloc_rr& __a )
		: __first( nullptr )
		, __begin( nullptr )
		, __end( nullptr )
		, __end_capm( nullptr, __a ) {}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 __split_buffer< _Tp, _Allocator >::__split_buffer( size_type __cap, size_type __start, __alloc_rr& __a )
		: __end_capm( nullptr, __a ) {
	if ( __cap == 0 ) __first = nullptr;
	else {
		auto __allocation = __allocate_at_least( __alloc(), __cap );
		__first           = __allocation.ptr;
		__cap             = __allocation.count;
	}
	__begin = __end = __first + __start;
	__end_cap()     = __first + __cap;
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 __split_buffer< _Tp, _Allocator >::__split_buffer( __split_buffer&& __c )
	LLVM_MSTL_NOEXCEPT_V( core::is_nothrow_move_constructible_v< allocator_type > )
		: __first( core::move( __c.__first ) )
		, __begin( core::move( __c.__begin ) )
		, __end( core::move( __c.__end ) )
		, __end_capm( core::move( __c.__end_capm ) ) {
	__c.__first     = nullptr;
	__c.__begin     = nullptr;
	__c.__end       = nullptr;
	__c.__end_cap() = nullptr;
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 __split_buffer< _Tp, _Allocator >::__split_buffer( __split_buffer&& __c, const __alloc_rr& __a )
		: __end_capm( nullptr, __a ) {
	if ( __a == __c.__alloc() ) {
		__first         = __c.__first;
		__begin         = __c.__begin;
		__end           = __c.__end;
		__end_cap()     = __c.__end_cap();
		__c.__first     = nullptr;
		__c.__begin     = nullptr;
		__c.__end       = nullptr;
		__c.__end_cap() = nullptr;
	} else {
		auto __allocation = __allocate_at_least( __alloc(), __c.size() );
		__first           = __allocation.ptr;
		__begin = __end = __first;
		__end_cap()     = __first + __allocation.count;
		using _Ip       = core::move_iterator< iterator >;
		__construct_at_end( _Ip( __c.begin() ), _Ip( __c.end() ) );
	}
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 __split_buffer< _Tp, _Allocator >::~__split_buffer() {
	clear();
	if ( __first )
		__alloc_traits::deallocate( __alloc(), __first, capacity() );
}

/**************************************		Operators		******************************************/
template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	__split_buffer< _Tp, _Allocator >&
	__split_buffer< _Tp, _Allocator >::operator=( __split_buffer< _Tp, _Allocator >&& __c )
		LLVM_MSTL_NOEXCEPT_V(
			(__alloc_traits::propagate_on_container_move_assignment::value &&
			 core::is_nothrow_move_assignable_v< allocator_type >) ||
			!__alloc_traits::propagate_on_container_move_assignment::value ) {
	clear();
	shrink_to_fit();
	__first     = __c.__first;
	__begin     = __c.__begin;
	__end       = __c.__end;
	__end_cap() = __c.__end_cap();
	__move_assign_alloc( __c, core::integral_constant< bool, __alloc_traits::propagate_on_container_move_assignment::value >() );
	__c.__first = __c.__begin = __c.__end = __c.__end_cap() = nullptr;
	return *this;
}

/*************************************		Functions		****************************************/
template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __split_buffer< _Tp, _Allocator >::reserve( size_type __n ) {
	if ( __n < capacity() ) {
		__split_buffer< value_type, __alloc_rr& > __t( __n, 0, __alloc() );
		__t.__construct_at_end(
			core::move_iterator< pointer >( __begin ),
			core::move_iterator< pointer >( __end ) );
		core::swap( __first, __t.__first );
		core::swap( __begin, __t.__begin );
		core::swap( __end, __t.__end );
		core::swap( __end_cap(), __t.__end_cap() );
	}
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __split_buffer< _Tp, _Allocator >::shrink_to_fit()
	LLVM_MSTL_NOEXCEPT {
	if ( capacity() > size() ) {
		__split_buffer< value_type, __alloc_rr& > __t( size(), 0, __alloc() );
		__t.__construct_at_end(
			core::move_iterator< pointer >( __begin ),
			core::move_iterator< pointer >( __end ) );
		__t.__end = __t.__begin + ( __t.__end - __begin );
		core::swap( __first, __t.__first );
		core::swap( __begin, __t.__begin );
		core::swap( __end, __t.__end );
		core::swap( __end_cap(), __t.__end_cap() );
	}
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __split_buffer< _Tp, _Allocator >::push_front( const_reference __x ) {
	if ( __begin == __first ) {
		if ( __end < __end_cap() ) {
			difference_type __d = __end_cap() - __end;
			__d                 = ( __d + 1 ) / 2;
			__begin             = core::move_backward( __begin, __end, __end + __d );
			__end += __d;
		} else {
			size_type __c =
				core::max< size_type >( 2 * static_cast< size_t >( __end_cap() - __first ), 1 );
			__split_buffer< value_type, __alloc_rr& > __t( __c, ( __c + 3 ) / 4, __alloc() );
			__t.__construct_at_end(
				core::move_iterator< pointer >( __begin ),
				core::move_iterator< pointer >( __end ) );
			core::swap( __first, __t.__first );
			core::swap( __begin, __t.__begin );
			core::swap( __end, __t.__end );
			core::swap( __end_cap(), __t.__end_cap() );
		}
	}
	__alloc_traits::construct( __alloc(), core::to_address( __begin - 1 ), __x );
	--__begin;
}

template < typename _Tp, typename _Alloctor >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __split_buffer< _Tp, _Alloctor >::push_front( value_type&& __x ) {
	if ( __begin == __first ) {
		if ( __end < __end_cap() ) {
			difference_type __d = __end_cap() - __end;
			__d                 = ( __d + 1 ) / 2;
			__begin             = core::move_backward( __begin, __end, __end + __d );
			__end += __d;
		} else {
			size_type __c =
				core::max< size_type >( 2 * static_cast< size_t >( __end_cap() - __first ), 1 );
			__split_buffer< value_type, __alloc_rr& > __t( __c, ( __c + 3 ) / 4, __alloc() );
			__t.__construct_at_end(
				core::move_iterator< pointer >( __begin ),
				core::move_iterator< pointer >( __end ) );
			core::swap( __first, __t.__first );
			core::swap( __begin, __t.__begin );
			core::swap( __end, __t.__end );
			core::swap( __end_cap(), __t.__end_cap() );
		}
	}
	__alloc_traits::construct( __alloc(), core::to_address( __begin - 1 ), core::move( __x ) );
	--__begin;
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	__split_buffer< _Tp, _Allocator >::push_back( const_reference __x ) {
	if ( __end == __end_cap() ) {
		if ( __begin > __first ) {
			difference_type __d = __begin - __first;
			__d                 = ( __d + 1 ) / 2;
			__end               = core::move( __begin, __end, __begin - __d );
			__begin             = __begin - __d;
		} else {
			size_type __c =
				core::max< size_type >( 2 * static_cast< size_t >( __end_cap() - __first ), 1 );
			__split_buffer< value_type, __alloc_rr& > __t( __c, __c / 4, __alloc() );
			__t.__construct_at_end(
				core::move_iterator< pointer >( __begin ),
				core::move_iterator< pointer >( __end ) );
			core::swap( __first, __t.__first );
			core::swap( __begin, __t.__begin );
			core::swap( __end, __t.__end );
			core::swap( __end_cap(), __t.__end_cap() );
		}
	}
	__alloc_traits::construct( __alloc(), core::to_address( __end ), __x );
	++__end;
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	__split_buffer< _Tp, _Allocator >::push_back( value_type&& __x ) {
	if ( __end == __end_cap() ) {
		if ( __begin > __first ) {
			difference_type __d = __begin - __first;
			__d                 = ( __d + 1 ) / 2;
			__end               = core::move( __begin, __end, __begin - __d );
			__begin             = __begin - __d;
		} else {
			size_type __c =
				core::max< size_type >( 2 * static_cast< size_t >( __end_cap() - __first ), 1 );
			__split_buffer< value_type, __alloc_rr& > __t( __c, __c / 4, __alloc() );
			__t.__construct_at_end(
				core::move_iterator< pointer >( __begin ),
				core::move_iterator< pointer >( __end ) );
			core::swap( __first, __t.__first );
			core::swap( __begin, __t.__begin );
			core::swap( __end, __t.__end );
			core::swap( __end_cap(), __t.__end_cap() );
		}
	}
	__alloc_traits::construct( __alloc(), core::to_address( __end ), core::move( __x ) );
	++__end;
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __split_buffer< _Tp, _Allocator >::swap( __split_buffer& __x )
	LLVM_MSTL_NOEXCEPT_V(
		!__alloc_traits::propagate_on_container_swap::value ||
		core::is_nothrow_swappable_v< __alloc_rr > ) {
	core::swap( __first, __x.__first );
	core::swap( __begin, __x.__begin );
	core::swap( __end, __x.__end );
	core::swap( __end_cap(), __x.__end_cap() );
	__swap_allocator( __alloc(), __x.__alloc() );
}

/********************************		Help Constructor		**************************************/
template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __split_buffer< _Tp, _Allocator >::__construct_at_end( size_type __n ) {
	_ConstructTransaction __tx( &this->__end, __n );
	for ( ; __tx.__pos != __tx.__end; ++__tx.__pos ) {
		__alloc_traits::construct( this->__alloc(), core::to_address( __tx.__pos ) );
	}
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __split_buffer< _Tp, _Allocator >::__construct_at_end( size_type __n, const_reference __x ) {
	_ConstructTransaction __tx( &this->__end, __n );
	for ( ; __tx.__pos != __tx.__end; ++__tx.__pos ) {
		__alloc_traits::construct( this->__alloc(), core::to_address( __tx.__pos ), __x );
	}
}

template < typename _Tp, typename _Allocator >
template < typename _InputIter >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __split_buffer< _Tp, _Allocator >::__construct_at_end( _InputIter __fst, _InputIter __last )
	-> core::enable_if_t< __is_exactly_cpp17_input_iterator< _InputIter >::value > {
	__alloc_rr& __a = this->__alloc();
	for ( ; __fst != __last; ++__fst ) {
		if ( __end == __end_cap() ) {
			size_type      __old_cap = __end_cap() - __first;
			size_type      __new_cap = core::max< size_type >( 2 * __old_cap, 8 );
			__split_buffer __buf( __new_cap, 0, __a );
			for ( pointer __p = __begin; __p != __end; ++__p, (void) ++__buf.__end ) {
				__alloc_traits::construct( __buf.__alloc(), core::to_address( __buf.__end ), core::move( *__p ) );
			}
			this->swap( __buf );//<--- Swapping with the new buffer to ensure proper memory management.
		}
		__alloc_traits::construct( __a, core::to_address( this->__end ), *__fst );
		++this->__end;
	}
}

template < typename _Tp, typename _Allocator >
template < typename _ForwardIter >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __split_buffer< _Tp, _Allocator >::__construct_at_end( _ForwardIter __fst, _ForwardIter __last )
	-> core::enable_if_t< __is_cpp17_forward_iterator< _ForwardIter >::value > {
	_ConstructTransaction __tx( &this->__end, (size_type) core::distance( __fst, __last ) );
	for ( ; __tx.__pos != __tx.__end; ++__tx.__pos, (void) ++__fst ) {
		__alloc_traits::construct( this->__alloc(), core::to_address( __tx.__pos ), *__fst );
	}
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	__split_buffer< _Tp, _Allocator >::__destruct_at_begin( pointer __new_begin, core::false_type ) {
	while ( __begin != __new_begin )
		__alloc_traits::destroy( __alloc(), core::to_address( __begin++ ) );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	__split_buffer< _Tp, _Allocator >::__destruct_at_begin( pointer __new_begin, core::true_type ) {
	__begin = __new_begin;
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	__split_buffer< _Tp, _Allocator >::__destruct_at_end( pointer __new_last, core::false_type ) LLVM_MSTL_NOEXCEPT {
	while ( __new_last != __end )
		__alloc_traits::destroy( __alloc(), core::to_address( --__end ) );
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20
	LLVM_MSTL_TEMPLATE_INLINE auto
	__split_buffer< _Tp, _Allocator >::__destruct_at_end( pointer __new_last, core::true_type ) LLVM_MSTL_NOEXCEPT {
	__end = __new_last;
}

template < typename _Tp, typename _Allocator >
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __split_buffer< _Tp, _Allocator >::__invariants() const -> bool {
	if ( __first == nullptr ) {
		if ( __begin != nullptr ) return false;
		if ( __end != nullptr ) return false;
		if ( __end_cap() != nullptr ) return false;
	} else {
		if ( __begin < __first ) return false;
		if ( __end < __begin ) return false;
		if ( __end_cap() < __end ) return false;
	}
}

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_SPLIT_BUFFER_H