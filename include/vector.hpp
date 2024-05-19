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
 *  template < class T, class Allocator = allocator<T> >
 *	class vector {
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
*			vector(initializer_list<value_type> il);
*			vector(initializer_list<value_type> il, const allocator_type& a);
*			~vector();
*			vector& operator=(const vector& x);
*			vector& operator=(vector&& x)
*						noexcept(
*								allocator_type::propagate_on_container_move_assignment::value ||
*								allocator_type::is_always_equal::value); // C++17
*				vector& operator=(initializer_list<value_type> il);
*				template <class InputIterator>
*						void assign(InputIterator first, InputIterator last);
				void assign(size_type n, const value_type& u);
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

	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 vector() = default;
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 explicit vector( const allocator_type& __a ) LLVM_MSTL_NOEXCEPT
	    : __end_cap( nullptr, __a ) {}

private:
	pointer                                      __begin = nullptr;
	pointer                                      __end   = nullptr;
	__compressed_pair< pointer, allocator_type > __end_cap =
	  __compressed_pair< pointer, allocator_type >( nullptr, __default_init_tag() );
};

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_VECTOR_H
