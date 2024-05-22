//
// Created by dezi on 5/19/2024.
//

#ifndef LLVM_MSTL___CONFIG_H
#define LLVM_MSTL___CONFIG_H

#define LLVM_MSTL_BEGIN_NAMESPACE_STD namespace nya {
#define LLVM_MSTL_END_NAMESPACE_STD   }

#define LLVM_MSTL_CORE_STD namespace core = std;

#ifdef __clang__
#define LLVM_MSTL_CLANG_VERSION 1
// #elifdef __gnu__
// #define LLVM_MSTL_GNU_VERSION 1
// #elifdef __msvc__
// #define LLVM_MSTL_MSVC_VERSION 1
#endif

#ifdef LLVM_MSTL_CLANG_VERSION
#define LLVM_MSTL_TEMPLATE_VIS    [[clang::type_visibility( "default" )]]
#define LLVM_MSTL_TEMPLATE_INLINE inline
#define LLVM_MSTL_NORETURN        [[noreturn]]
#define LLVM_MSTL_NODISCARD       [[nodiscard]]
#define LLVM_MSTL_INLINE          inline
// #elifdef LLVM_MSTL_GNU_VERSION
// #elifdef LLVM_MSTL_MSVC_VERSION
#endif

#ifndef LLVM_MSTL_STD_VERSION
#if __cplusplus <= 201103L
#define LLVM_MSTL_STD_VERSION 11
#elif __cplusplus <= 201402L
#define LLVM_MSTL_STD_VERSION 14
#elif __cplusplus <= 201703L
#define LLVM_MSTL_STD_VERSION 17
#elif __cplusplus >= 202002L
#define LLVM_MSTL_STD_VERSION 20
#endif
#endif

#if LLVM_MSTL_STD_VERSION >= 20
#define LLVM_MSTL_CONSTEXPR_SINCE_CXX20 constexpr
#else
#define LLVM_MSTL_CONSTEXPR_SINCE_CXX20
#endif

#define LLVM_MSTL_CONSTEXPR       constexpr
#define LLVM_MSTL_NOEXCEPT        noexcept
#define LLVM_MSTL_NOEXCEPT_V( x ) noexcept( x )

#if LLVM_MSTL_STD_VERSION >= 17
/**
 * @brief CTAD（Class Template Argument Deduction）是C++17引入的一项特性，用于在实例化类模板时自动推导模板参数。在C++之前，实例化类模板时必须显式提供所有模板参数，而CTAD使得在某些情况下可以省略模板参数的显式指定。
 * CTAD的主要目的是简化代码和提高代码的可读性。它可以在特定条件下自动推导类模板的模板参数，从而减少代码中的冗余和重复。
 * 以下是CTAD的一些基本规则和用法：
 * CTAD适用于类模板，而不适用于函数模板。
 * CTAD只能在使用类模板的初始化过程中进行推导，例如通过直接初始化、拷贝初始化或列表初始化来创建对象。
 * CTAD的推导过程是在编译时完成的，不会影响运行时的性能。
 * CTAD的推导结果取决于构造函数的形参列表和实参列表之间的匹配程度。
 * CTAD不会推导默认模板参数，因此必须显式指定默认模板参数或使用模板别名来处理默认模板参数。
 * 
 */
#define LLVM_MSTL_CTAD_SUPPORTED_FOR_TYPE( _ClassName ) \
	template < typename... _Tag >                         \
	_ClassName( typename _Tag::__allow_ctad... )->_ClassName< _Tag... >
#endif

#endif//LLVM_MSTL___CONFIG_H
