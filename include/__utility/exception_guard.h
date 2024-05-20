#ifndef LLVM_MSTL_EXCEPTION_GUARD_H
#define LLVM_MSTL_EXCEPTION_GUARD_H

#include "__config.h"

#include <type_traits>
#include <utility>

LLVM_MSTL_BEGIN_NAMESPACE_STD

LLVM_MSTL_CORE_STD

/**
 * @brief Exception guard for rolling back operations in case of exceptions.
 *
 * This struct provides an exception guard for rolling back operations in case of exceptions.
 * It is used to manage resources and ensure proper rollback when exceptions occur.
 *
 * @tparam _Rollback The type of the rollback function or callable object.
 */
template < typename _Rollback >
struct __exception_guard_exceptions {
	/**
    * @brief Deleted default constructor.
    *
    * The default constructor is deleted to prevent default initialization of the guard.
    */
	__exception_guard_exceptions() = delete;

	/**
    * @brief Explicit constructor for initializing the exception guard.
    *
    * This constructor initializes the exception guard with the specified rollback function or callable object.
    *
    * @param __rollback The rollback function or callable object.
    */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 explicit __exception_guard_exceptions( _Rollback __rollback )
	    : __rollback( core::move( __rollback ) )
	    , __completed( false ) {}

	/**
    * @brief Move constructor for the exception guard.
    *
    * This move constructor moves the contents from another exception guard object.
    * It is declared as noexcept if the rollback function or callable object is nothrow move assignable.
    *
    * @param __other The other exception guard to be moved.
    */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 __exception_guard_exceptions( __exception_guard_exceptions&& __other )
	  LLVM_MSTL_NOEXCEPT_V( core::is_nothrow_move_assignable_v< _Rollback > )
	    : __rollback( core::move( __other.__rollback ) )
	    , __completed( __other.__completed ) {
		__other.__completed = true;
	}

	//<--- Deleted copy constructor and assignment operators
	__exception_guard_exceptions( __exception_guard_exceptions const& )            = delete;
	__exception_guard_exceptions& operator=( __exception_guard_exceptions const& ) = delete;
	__exception_guard_exceptions& operator=( __exception_guard_exceptions&& )      = delete;

	/**
    * @brief Marks the exception guard as completed.
    *
    * This function marks the exception guard as completed, indicating that the guarded operation has finished successfully.
    * It is intended to be called when the guarded operation completes without any exceptions.
    *
    * @return void
    */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 auto __complete() LLVM_MSTL_NOEXCEPT {
		__completed = true;
	}

	/**
    * @brief Destructor for the exception guard.
    *
    * The destructor checks if the exception guard has been completed.
    * If it hasn't, it calls the rollback function or callable object to perform the necessary cleanup.
    */
	LLVM_MSTL_CONSTEXPR_SINCE_CXX20 ~__exception_guard_exceptions() {
		if ( !__completed ) __rollback();
	}

private:
	_Rollback __rollback; //<--- The rollback function or callable object
	bool      __completed;//<--- Flag indicating if the guarded operation has completed
};


LLVM_MSTL_CTAD_SUPPORTED_FOR_TYPE( __exception_guard_exceptions );

/**
 * @brief Alias template for __exception_guard_exceptions.
 *
 * This alias template provides a shorter name for __exception_guard_exceptions with a given _Rollback type.
 *
 * @tparam _Rollback The type of the rollback function or callable object.
 */
template < typename _Rollback >
using __exception_guard = __exception_guard_exceptions< _Rollback >;

/**
 * @brief Creates an exception guard object.
 *
 * This function template creates an exception guard object with the specified rollback function or callable object.
 *
 * @tparam _Rollback The type of the rollback function or callable object.
 * @param __rollback The rollback function or callable object.
 * @return The created exception guard object.
 */
template < typename _Rollback >
LLVM_MSTL_CONSTEXPR auto __make_exception_guard( _Rollback __rollback ) -> __exception_guard< _Rollback > {
	return __exception_guard< _Rollback >( core::move( __rollback ) );
}

LLVM_MSTL_END_NAMESPACE_STD

#endif//LLVM_MSTL_EXCEPTION_GUARD_H