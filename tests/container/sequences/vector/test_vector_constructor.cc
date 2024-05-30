#include "__iterator/wrap_iter.h"
#include "vector.hpp"
#include "gtest/gtest.h"

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <random>
#include <stdint.h>

static core::random_device                       rd;
static core::mt19937                             generator( rd() );
static core::uniform_int_distribution< int64_t > distribution( 0 );

TEST( VECTOR_CONSTRUCTOR, default_construct ) {
	nya::vector< int64_t, core::allocator< int64_t > > __v{};

	ASSERT_EQ( 0, __v.size() );
	ASSERT_EQ( 0, __v.capacity() );

	__v.~vector();
}

TEST( VECTOR_CONSTRUCTOR, allocator_construct ) {
	core::allocator< int64_t >                         __a;
	nya::vector< int64_t, core::allocator< int64_t > > __v{ __a };

	ASSERT_EQ( 0, __v.size() );
	ASSERT_EQ( 0, __v.capacity() );
}

TEST( VECTOR_CONSTRUCTOR, size_construct ) {
	uint64_t                                           __v_size = 1000000;
	nya::vector< int64_t, core::allocator< int64_t > > __v( __v_size );

	using pointer = typename core::allocator_traits< core::allocator< int64_t > >::pointer;
	nya::__wrap_iter< pointer > __wi;
	ASSERT_TRUE( __v.begin() != __wi );
	ASSERT_TRUE( __v.end() != __wi );
	ASSERT_TRUE( __v_size == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.size() == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.capacity() == __v_size );
}

TEST( VECTOR_CONSTRUCTOR, size_construct_with_allocator ) {
	uint64_t                                           __v_size = 1000000;
	core::allocator< int64_t >                         __a;
	nya::vector< int64_t, core::allocator< int64_t > > __v( __v_size, __a );

	using pointer = typename core::allocator_traits< core::allocator< int64_t > >::pointer;
	nya::__wrap_iter< pointer > __wi;
	ASSERT_TRUE( __v.begin() != __wi );
	ASSERT_TRUE( __v.end() != __wi );
	ASSERT_TRUE( __v_size == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.size() == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.capacity() == __v_size );
}

TEST( VECTOR_CONSTRUCTOR, size_construct_with_value ) {
	uint64_t                                           __v_size = 1000000;
	core::uniform_int_distribution< int64_t >          __dis( 0, (int64_t) __v_size );
	int64_t                                            __init_var = __dis( generator );
	nya::vector< int64_t, core::allocator< int64_t > > __v( __v_size, __init_var );

	using pointer = typename core::allocator_traits< core::allocator< int64_t > >::pointer;
	nya::__wrap_iter< pointer > __wi;
	ASSERT_TRUE( __v.begin() != __wi );
	ASSERT_TRUE( __v.end() != __wi );
	ASSERT_TRUE( __v_size == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.size() == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.capacity() == __v_size );
	ASSERT_TRUE( __init_var == *__v.begin() );
	ASSERT_TRUE( __init_var == *( __v.end() - 1 ) );

	ASSERT_TRUE( __init_var == __v.at( static_cast< uint64_t >( __dis( generator ) ) ) );

	for ( size_t i = 0; i < __v_size; i++ ) {
		ASSERT_EQ( __init_var, __v.at( i ) );
	}
}

TEST( VECTOR_CONSTRUCTOR, size_construct_with_value_and_allocator ) {
	uint64_t                                           __v_size = 1000000;
	core::uniform_int_distribution< int64_t >          __dis( 0, (int64_t) __v_size );
	int64_t                                            __init_var = __dis( generator );
	core::allocator< int64_t >                         __a;
	nya::vector< int64_t, core::allocator< int64_t > > __v( __v_size, __init_var, __a );

	using pointer = typename core::allocator_traits< core::allocator< int64_t > >::pointer;
	nya::__wrap_iter< pointer > __wi;
	ASSERT_TRUE( __v.begin() != __wi );
	ASSERT_TRUE( __v.end() != __wi );
	ASSERT_TRUE( __v_size == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.size() == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.capacity() == __v_size );
	ASSERT_TRUE( __init_var == *__v.begin() );
	ASSERT_TRUE( __init_var == *( __v.end() - 1 ) );

	ASSERT_TRUE( __init_var == __v.at( static_cast< uint64_t >( __dis( generator ) ) ) );

	for ( size_t i = 0; i < __v_size; i++ ) {
		ASSERT_EQ( __init_var, __v.at( i ) );
	}
}

TEST( VECTOR_CONSTRUCTOR, input_iter_construct ) {
	uint64_t                                  __v_size = 1000000;
	core::uniform_int_distribution< int64_t > __dis( 0, (int64_t) __v_size );
	std::unique_ptr< int64_t[] >              __v_range( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v( __v_range.get(), __v_range.get() + __v_size );

	using pointer = typename core::allocator_traits< core::allocator< int64_t > >::pointer;
	nya::__wrap_iter< pointer > __wi;
	ASSERT_TRUE( __v.begin() != __wi );
	ASSERT_TRUE( __v.end() != __wi );
	ASSERT_TRUE( __v_size == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.size() == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.capacity() == __v_size );
	ASSERT_TRUE( __v_range[ 0 ] == *__v.begin() );
	ASSERT_TRUE( __v_range[ __v_size - 1 ] == *( __v.end() - 1 ) );

	for ( size_t i = 0; i < __v_size; i++ ) {
		ASSERT_EQ( __v_range[ i ], __v.at( i ) );
	}
}

TEST( VECTOR_CONSTRUCTOR, input_iter_construct_with_allocator ) {
	uint64_t                                  __v_size = 1000000;
	core::allocator< int64_t >                __a;
	core::uniform_int_distribution< int64_t > __dis( 0, (int64_t) __v_size );
	std::unique_ptr< int64_t[] >              __v_range( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v(
		__v_range.get(), __v_range.get() + __v_size, __a );

	using pointer = typename core::allocator_traits< core::allocator< int64_t > >::pointer;
	nya::__wrap_iter< pointer > __wi;
	ASSERT_TRUE( __v.begin() != __wi );
	ASSERT_TRUE( __v.end() != __wi );
	ASSERT_TRUE( __v_size == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.size() == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.capacity() == __v_size );
	ASSERT_TRUE( __v_range[ 0 ] == *__v.begin() );
	ASSERT_TRUE( __v_range[ __v_size - 1 ] == *( __v.end() - 1 ) );

	for ( size_t i = 0; i < __v_size; i++ ) {
		ASSERT_EQ( __v_range[ i ], __v.at( i ) );
	}
}

TEST( VECTOR_CONSTRUCTOR, forward_iter_construct ) {
	uint64_t                                  __v_size = 1000000;
	core::allocator< int64_t >                __a;
	core::uniform_int_distribution< int64_t > __dis( 0, (int64_t) __v_size );
	std::unique_ptr< int64_t[] >              __v_range( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v(
		__v_range.get(), __v_range.get() + __v_size, __a );

	nya::vector< int64_t, core::allocator< int64_t > > __v_forward( __v.begin(), __v.end() );

	using pointer = typename core::allocator_traits< core::allocator< int64_t > >::pointer;
	nya::__wrap_iter< pointer > __wi;
	ASSERT_TRUE( __v_forward.begin() != __wi );
	ASSERT_TRUE( __v_forward.end() != __wi );
	ASSERT_TRUE( __v_size == static_cast< uint64_t >( __v_forward.end() - __v_forward.begin() ) );
	ASSERT_TRUE( __v_forward.size() == static_cast< uint64_t >( __v_forward.end() - __v_forward.begin() ) );
	ASSERT_TRUE( __v_forward.capacity() == __v_size );
	ASSERT_TRUE( __v_range[ 0 ] == *__v_forward.begin() );
	ASSERT_TRUE( __v_range[ __v_size - 1 ] == *( __v_forward.end() - 1 ) );

	for ( size_t i = 0; i < __v_size; i++ ) {
		ASSERT_EQ( __v_range[ i ], __v_forward.at( i ) );
	}
}

TEST( VECTOR_CONSTRUCTOR, forward_iter_construct_with_allocator ) {
	uint64_t                                  __v_size = 1000000;
	core::allocator< int64_t >                __a;
	core::uniform_int_distribution< int64_t > __dis( 0, (int64_t) __v_size );
	std::unique_ptr< int64_t[] >              __v_range( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v(
		__v_range.get(), __v_range.get() + __v_size, __a );

	nya::vector< int64_t, core::allocator< int64_t > > __v_forward( __v.begin(), __v.end(), __a );

	using pointer = typename core::allocator_traits< core::allocator< int64_t > >::pointer;
	nya::__wrap_iter< pointer > __wi;
	ASSERT_TRUE( __v_forward.begin() != __wi );
	ASSERT_TRUE( __v_forward.end() != __wi );
	ASSERT_TRUE( __v_size == static_cast< uint64_t >( __v_forward.end() - __v_forward.begin() ) );
	ASSERT_TRUE( __v_forward.size() == static_cast< uint64_t >( __v_forward.end() - __v_forward.begin() ) );
	ASSERT_TRUE( __v_forward.capacity() == __v_size );
	ASSERT_TRUE( __v_range[ 0 ] == *__v_forward.begin() );
	ASSERT_TRUE( __v_range[ __v_size - 1 ] == *( __v_forward.end() - 1 ) );

	for ( size_t i = 0; i < __v_size; i++ ) {
		ASSERT_EQ( __v_range[ i ], __v_forward.at( i ) );
	}
}

TEST( VECTOR_CONSTRUCTOR, copy_construct ) {
	uint64_t                                  __v_size = 1000000;
	core::allocator< int64_t >                __a;
	core::uniform_int_distribution< int64_t > __dis( 0, (int64_t) __v_size );
	std::unique_ptr< int64_t[] >              __v_range( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v(
		__v_range.get(), __v_range.get() + __v_size, __a );

	nya::vector< int64_t, core::allocator< int64_t > > __v_copy( __v );

	using pointer = typename core::allocator_traits< core::allocator< int64_t > >::pointer;
	nya::__wrap_iter< pointer > __wi;
	ASSERT_TRUE( __v_copy.begin() != __wi );
	ASSERT_TRUE( __v_copy.end() != __wi );
	ASSERT_TRUE( __v_size == static_cast< uint64_t >( __v_copy.end() - __v_copy.begin() ) );
	ASSERT_TRUE( __v_copy.size() == static_cast< uint64_t >( __v_copy.end() - __v_copy.begin() ) );
	ASSERT_TRUE( __v_copy.capacity() == __v_size );
	ASSERT_TRUE( __v_range[ 0 ] == *__v_copy.begin() );
	ASSERT_TRUE( __v_range[ __v_size - 1 ] == *( __v_copy.end() - 1 ) );

	for ( size_t i = 0; i < __v_size; i++ ) {
		ASSERT_EQ( __v.at( i ), __v_copy.at( i ) );
	}
}

TEST( VECTOR_CONSTRUCTOR, copy_construct_with_alloctor ) {
	uint64_t                                  __v_size = 1000000;
	core::allocator< int64_t >                __a;
	core::uniform_int_distribution< int64_t > __dis( 0, (int64_t) __v_size );
	std::unique_ptr< int64_t[] >              __v_range( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v(
		__v_range.get(), __v_range.get() + __v_size, __a );

	nya::vector< int64_t, core::allocator< int64_t > > __v_copy( __v, __a );

	using pointer = typename core::allocator_traits< core::allocator< int64_t > >::pointer;
	nya::__wrap_iter< pointer > __wi;
	ASSERT_TRUE( __v_copy.begin() != __wi );
	ASSERT_TRUE( __v_copy.end() != __wi );
	ASSERT_TRUE( __v_size == static_cast< uint64_t >( __v_copy.end() - __v_copy.begin() ) );
	ASSERT_TRUE( __v_copy.size() == static_cast< uint64_t >( __v_copy.end() - __v_copy.begin() ) );
	ASSERT_TRUE( __v_copy.capacity() == __v_size );
	ASSERT_TRUE( __v_range[ 0 ] == *__v_copy.begin() );
	ASSERT_TRUE( __v_range[ __v_size - 1 ] == *( __v_copy.end() - 1 ) );

	for ( size_t i = 0; i < __v_size; i++ ) {
		ASSERT_EQ( __v.at( i ), __v_copy.at( i ) );
	}
}

TEST( VECTOR_CONSTRUCTOR, inintialize_construct ) {
	nya::vector< int64_t, core::allocator< int64_t > > __v_test(
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } );

	EXPECT_EQ( 1, *__v_test.begin() );
	EXPECT_EQ( 10, *( __v_test.end() - 1 ) );
	EXPECT_EQ( 10, __v_test.size() );
	EXPECT_EQ( 10, __v_test.capacity() );
}

TEST( VECTOR_CONSTRUCTOR, inintialize_construct_with_allocator ) {
	core::allocator< int64_t >                         __a;
	nya::vector< int64_t, core::allocator< int64_t > > __v_test(
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }, __a );

	EXPECT_EQ( 1, *__v_test.begin() );
	EXPECT_EQ( 10, *( __v_test.end() - 1 ) );
	EXPECT_EQ( 10, __v_test.size() );
	EXPECT_EQ( 10, __v_test.capacity() );
}

TEST( VECTOR_CONSTRUCTOR, move_construct ) {
	uint64_t                                  __v_size = 1000000;
	core::uniform_int_distribution< int64_t > __dis( 0, (int64_t) __v_size );
	std::unique_ptr< int64_t[] >              __v_range( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v__move( __v_range.get(), __v_range.get() + __v_size );

	nya::vector< int64_t, core::allocator< int64_t > > __v( core::move( __v__move ) );

	ASSERT_EQ( nullptr, __v__move.begin().base() );
	ASSERT_EQ( nullptr, __v__move.end().base() );
	ASSERT_EQ( 0, __v__move.size() );
	ASSERT_EQ( 0, __v__move.capacity() );

	using pointer = typename core::allocator_traits< core::allocator< int64_t > >::pointer;
	nya::__wrap_iter< pointer > __wi;
	ASSERT_TRUE( __v.begin() != __wi );
	ASSERT_TRUE( __v.end() != __wi );
	ASSERT_TRUE( __v_size == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.size() == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.capacity() == __v_size );
	ASSERT_TRUE( __v_range[ 0 ] == *__v.begin() );
	ASSERT_TRUE( __v_range[ __v_size - 1 ] == *( __v.end() - 1 ) );

	for ( size_t i = 0; i < __v_size; i++ ) {
		ASSERT_EQ( __v_range[ i ], __v.at( i ) );
	}
}

TEST( VECTOR_CONSTRUCTOR, move_construct_with_allocator ) {
	uint64_t                                  __v_size = 1000000;
	core::uniform_int_distribution< int64_t > __dis( 0, (int64_t) __v_size );
	std::unique_ptr< int64_t[] >              __v_range( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v__move( __v_range.get(), __v_range.get() + __v_size );

	core::allocator< int64_t >                         __a;
	nya::vector< int64_t, core::allocator< int64_t > > __v( core::move( __v__move ), __a );

	ASSERT_EQ( nullptr, __v__move.begin().base() );
	ASSERT_EQ( nullptr, __v__move.end().base() );
	ASSERT_EQ( 0, __v__move.size() );
	ASSERT_EQ( 0, __v__move.capacity() );

	using pointer = typename core::allocator_traits< core::allocator< int64_t > >::pointer;
	nya::__wrap_iter< pointer > __wi;
	ASSERT_TRUE( __v.begin() != __wi );
	ASSERT_TRUE( __v.end() != __wi );
	ASSERT_TRUE( __v_size == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.size() == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.capacity() == __v_size );
	ASSERT_TRUE( __v_range[ 0 ] == *__v.begin() );
	ASSERT_TRUE( __v_range[ __v_size - 1 ] == *( __v.end() - 1 ) );

	for ( size_t i = 0; i < __v_size; i++ ) {
		ASSERT_EQ( __v_range[ i ], __v.at( i ) );
	}
}

GTEST_API_ int main( int argc, char* argv[] ) {
	testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
