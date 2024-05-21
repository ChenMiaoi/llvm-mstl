#ifndef LLVM_MSTL_SPLIT_BUFFER_H
#define LLVM_MSTL_SPLIT_BUFFER_H

#include "__config.h"
#include "__memory/allocate_at_least.h"
#include "__memory/compress_pair.h"

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
  * @brief Destructor for the __split_buffer.
  *
  * This destructor cleans up the __split_buffer object by clearing its contents and deallocating the memory if necessary.
  *
  * @tparam _Tp The type of objects stored in the buffer.
  * @tparam _Allocator The allocator type used for memory management.
  * @throws None
  */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 ~__split_buffer();

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto clear() LLVM_MSTL_NOEXCEPT;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto capacity() const;
};

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
LLVM_MSTL_CONSTEXPR_SINCE_CXX20 __split_buffer< _Tp, _Allocator >::~__split_buffer() {
	clear();
	if ( __first )
		__alloc_traits::deallocate( __alloc(), __first, capacity() );
}

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_SPLIT_BUFFER_H