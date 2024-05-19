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
#elifdef __gnu__
#define LLVM_MSTL_GNU_VERSION 1
#elifdef __msvc__
#define LLVM_MSTL_MSVC_VERSION 1
#endif

#ifdef LLVM_MSTL_CLANG_VERSION
#define LLVM_MSTL_TEMPLATE_VIS [[clang::type_visibility( "default" )]]
#elifdef LLVM_MSTL_GNU_VERSION
#elifdef LLVM_MSTL_MSVC_VERSION
#endif

#ifndef LLVM_MSTL_STD_VERSION
#if __cplusplus <= 201103L
#define LLVM_MSTL_STD_VERSION 202002L
#elif __cplusplus <= 201402L
#define LLVM_MSTL_STD_VERSION 202002L
#elif __cplusplus <= 201703L
#define LLVM_MSTL_STD_VERSION 202002L
#elif __cplusplus >= 202002L
#define LLVM_MSTL_STD_VERSION 202002L
#endif
#endif

#if LLVM_MSTL_STD_VERSION >= 202002L
#define LLVM_MSTL_CONSTEXPR_SINCE_CXX20 constexpr
#else
#define LLVM_MSTL_CONSTEXPR_SINCE_CXX20
#endif

#define LLVM_MSTL_NOEXCEPT        noexcept
#define LLVM_MSTL_NOEXCEPT_V( x ) noexcept( x )

#endif//LLVM_MSTL___CONFIG_H
