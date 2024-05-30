//
// Created by dezi on 5/19/2024.
//

#ifndef LLVM_MSTL_WRAP_ITER_H
#define LLVM_MSTL_WRAP_ITER_H

#include "__config.h"


#include <iterator>
#include <memory>
#include <type_traits>

LLVM_MSTL_BEGIN_NAMESPACE_STD
LLVM_MSTL_CORE_STD

template < typename _Iter >
class __wrap_iter {
public:
	using iterator_type     = _Iter;
	using value_type        = typename core::iterator_traits< iterator_type >::value_type;
	using difference_type   = typename core::iterator_traits< iterator_type >::difference_type;
	using pointer           = typename core::iterator_traits< iterator_type >::pointer;
	using reference         = typename core::iterator_traits< iterator_type >::reference;
	using iterator_category = typename core::iterator_traits< iterator_type >::iterator_category;

private:
	iterator_type __it;

private:
	LLVM_MSTL_CONSTEXPR explicit __wrap_iter( const void* __p, iterator_type __x ) LLVM_MSTL_NOEXCEPT
			: __it( __x ) {
		static_cast< void >( __p );
	}

	template < typename _Up >
	friend class __wrap_iter;
	template < class _CharT, class _Traits, class _Alloc >
	friend class basic_string;
	template < class _Tp, class _Alloc >
	friend class vector;
	template < class _Tp, size_t >
	friend class span;

public:
	LLVM_MSTL_CONSTEXPR
	__wrap_iter() LLVM_MSTL_NOEXCEPT
			: __it() {}

	template < typename _Up >
	LLVM_MSTL_CONSTEXPR __wrap_iter(
		const __wrap_iter< _Up >& __u,
		typename core::enable_if< core::is_convertible_v< _Up, iterator_type > >::type* = nullptr )
		LLVM_MSTL_NOEXCEPT
			: __it( __u.base() ) {}

	LLVM_MSTL_CONSTEXPR reference operator*() const LLVM_MSTL_NOEXCEPT {
		return *__it;
	}

	LLVM_MSTL_CONSTEXPR pointer operator->() const LLVM_MSTL_NOEXCEPT {
		return core::to_address( __it );
	}

	LLVM_MSTL_CONSTEXPR __wrap_iter& operator++() LLVM_MSTL_NOEXCEPT {
		++__it;
		return *this;
	}

	LLVM_MSTL_CONSTEXPR __wrap_iter operator++( int ) LLVM_MSTL_NOEXCEPT {
		__wrap_iter __tmp( *this );
		++( *this );
		return __tmp;
	}

	LLVM_MSTL_CONSTEXPR __wrap_iter& operator--() LLVM_MSTL_NOEXCEPT {
		--__it;
		return *this;
	}

	LLVM_MSTL_CONSTEXPR __wrap_iter operator--( int ) LLVM_MSTL_NOEXCEPT {
		__wrap_iter __tmp( *this );
		--( *this );
		return __tmp;
	}

	LLVM_MSTL_CONSTEXPR __wrap_iter operator+( difference_type __n ) const LLVM_MSTL_NOEXCEPT {
		__wrap_iter __tmp( *this );
		__tmp += __n;
		return __tmp;
	}

	LLVM_MSTL_CONSTEXPR __wrap_iter& operator+=( difference_type __n ) LLVM_MSTL_NOEXCEPT {
		__it += __n;
		return *this;
	}

	LLVM_MSTL_CONSTEXPR __wrap_iter operator-( difference_type __n ) const LLVM_MSTL_NOEXCEPT {
		__wrap_iter __tmp( *this );
		__tmp -= __n;
		return __tmp;
	}

	LLVM_MSTL_CONSTEXPR __wrap_iter& operator-=( difference_type __n ) LLVM_MSTL_NOEXCEPT {
		__it -= __n;
		return *this;
	}

	LLVM_MSTL_CONSTEXPR reference operator[]( difference_type __n ) const LLVM_MSTL_NOEXCEPT {
		return __it[ __n ];
	}

	LLVM_MSTL_CONSTEXPR iterator_type base() const LLVM_MSTL_NOEXCEPT {
		return __it;
	}
};

template < typename _Iter1 >
LLVM_MSTL_CONSTEXPR bool
operator==( const __wrap_iter< _Iter1 >& __x, const __wrap_iter< _Iter1 >& __y ) LLVM_MSTL_NOEXCEPT {
	return __x.base() == __y.base();
}

template < typename _Iter1, typename _Iter2 >
LLVM_MSTL_CONSTEXPR bool
operator==( const __wrap_iter< _Iter1 >& __x, const __wrap_iter< _Iter2 >& __y ) LLVM_MSTL_NOEXCEPT {
	return __x.base() == __y.base();
}

template < typename _Iter1 >
LLVM_MSTL_CONSTEXPR bool
operator!=( const __wrap_iter< _Iter1 >& __x, const __wrap_iter< _Iter1 >& __y ) LLVM_MSTL_NOEXCEPT {
	return !( __x == __y );
}

template < typename _Iter1, typename _Iter2 >
LLVM_MSTL_CONSTEXPR bool
operator!=( const __wrap_iter< _Iter1 >& __x, const __wrap_iter< _Iter2 >& __y ) LLVM_MSTL_NOEXCEPT {
	return !( __x == __y );
}

template < typename _Iter1 >
LLVM_MSTL_CONSTEXPR bool
operator<( const __wrap_iter< _Iter1 >& __x, const __wrap_iter< _Iter1 >& __y ) LLVM_MSTL_NOEXCEPT {
	return __x.base() < __y.base();
}

template < typename _Iter1, typename _Iter2 >
LLVM_MSTL_CONSTEXPR bool
operator<( const __wrap_iter< _Iter1 >& __x, const __wrap_iter< _Iter2 >& __y ) LLVM_MSTL_NOEXCEPT {
	return __x.base() < __y.base();
}

template < typename _Iter1 >
LLVM_MSTL_CONSTEXPR bool
operator>( const __wrap_iter< _Iter1 >& __x, const __wrap_iter< _Iter1 >& __y ) LLVM_MSTL_NOEXCEPT {
	return __y < __x;
}

template < typename _Iter1, typename _Iter2 >
LLVM_MSTL_CONSTEXPR bool
operator>( const __wrap_iter< _Iter1 >& __x, const __wrap_iter< _Iter2 >& __y ) LLVM_MSTL_NOEXCEPT {
	return __y < __x;
}

template < typename _Iter1 >
LLVM_MSTL_CONSTEXPR bool
operator<=( const __wrap_iter< _Iter1 >& __x, const __wrap_iter< _Iter1 >& __y ) LLVM_MSTL_NOEXCEPT {
	return !( __y < __x );
}

template < typename _Iter1, typename _Iter2 >
LLVM_MSTL_CONSTEXPR bool
operator<=( const __wrap_iter< _Iter1 >& __x, const __wrap_iter< _Iter2 >& __y ) LLVM_MSTL_NOEXCEPT {
	return !( __y < __x );
}

template < typename _Iter1 >
LLVM_MSTL_CONSTEXPR bool
operator>=( const __wrap_iter< _Iter1 >& __x, const __wrap_iter< _Iter1 >& __y ) LLVM_MSTL_NOEXCEPT {
	return !( __x < __y );
}

template < typename _Iter1, typename _Iter2 >
LLVM_MSTL_CONSTEXPR bool
operator>=( const __wrap_iter< _Iter1 >& __x, const __wrap_iter< _Iter2 >& __y ) LLVM_MSTL_NOEXCEPT {
	return !( __x < __y );
}

template < typename _Iter1, typename _Iter2 >
LLVM_MSTL_CONSTEXPR auto operator-( const __wrap_iter< _Iter1 >& __x, const __wrap_iter< _Iter2 >& __y )
	LLVM_MSTL_NOEXCEPT->decltype( __x.base() - __y.base() ) {
	return __x.base() - __y.base();
}

template < typename _Iter1 >
LLVM_MSTL_CONSTEXPR __wrap_iter< _Iter1 > operator+(
	typename __wrap_iter< _Iter1 >::difference_type __n,
	const __wrap_iter< _Iter1 >&                    __x )
	LLVM_MSTL_NOEXCEPT {
	__x += __n;
	return __x;
}

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_WRAP_ITER_H
