#include "__iterator/wrap_iter.h"
#include "vector.hpp"
#include "gtest/gtest.h"

#include <algorithm>
#include <initializer_list>
#include <limits>
#include <memory>
#include <random>
#include <stdint.h>

static core::random_device                       rd;
static core::mt19937                             generator( rd() );
static core::uniform_int_distribution< int64_t > distribution( 0, core::numeric_limits< int64_t >::max() - 1 );

TEST( VECTOR_OEPRATOR, copy_operator ) {
	// TODO
}

TEST( VECTOR_OEPRATOR, inintialize_operator ) {
	// TODO
}

TEST( VECTOR_OEPRATOR, move_operator ) {
	// TODO
}

TEST( VECTOR_OEPRATOR, index_operator ) {
	uint64_t                     __v_size = 1000000;
	std::unique_ptr< int64_t[] > __v_range( new int64_t[ __v_size ]{} );
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
	ASSERT_TRUE( __v_range[ 0 ] == __v[ 0 ] );
	ASSERT_TRUE( __v_range[ __v_size - 1 ] == __v[ __v_size - 1 ] );

	for ( size_t i = 0; i < __v_size; i++ ) {
		ASSERT_EQ( __v_range[ i ], __v.at( i ) );
	}
}

TEST( VECTOR_OEPRATOR, const_index_operator ) {
	uint64_t                     __v_size = 1000000;
	std::unique_ptr< int64_t[] > __v_range( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	const nya::vector< int64_t, core::allocator< int64_t > > __v( __v_range.get(), __v_range.get() + __v_size );

	using pointer = typename core::allocator_traits< core::allocator< int64_t > >::pointer;
	nya::__wrap_iter< pointer > __wi;
	ASSERT_TRUE( __v.begin() != __wi );
	ASSERT_TRUE( __v.end() != __wi );
	ASSERT_TRUE( __v_size == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.size() == static_cast< uint64_t >( __v.end() - __v.begin() ) );
	ASSERT_TRUE( __v.capacity() == __v_size );
	ASSERT_TRUE( __v_range[ 0 ] == *__v.begin() );
	ASSERT_TRUE( __v_range[ __v_size - 1 ] == *( __v.end() - 1 ) );
	ASSERT_TRUE( __v_range[ 0 ] == __v[ 0 ] );
	ASSERT_TRUE( __v_range[ __v_size - 1 ] == __v[ __v_size - 1 ] );

	for ( size_t i = 0; i < __v_size; i++ ) {
		ASSERT_EQ( __v_range[ i ], __v.at( i ) );
	}
}

TEST( VECTOR_OEPRATOR, equal_operator ) {
	uint64_t                     __v_size = 1000000;
	std::unique_ptr< int64_t[] > __v_range( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v1( __v_range.get(), __v_range.get() + __v_size );
	nya::vector< int64_t, core::allocator< int64_t > > __v2( __v1.begin(), __v1.end() );

	ASSERT_TRUE( __v1 == __v2 );
}

TEST( VECTOR_OEPRATOR, const_equal_operator ) {
	uint64_t                     __v_size = 1000000;
	std::unique_ptr< int64_t[] > __v_range( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	const nya::vector< int64_t, core::allocator< int64_t > > __v1( __v_range.get(), __v_range.get() + __v_size );
	const nya::vector< int64_t, core::allocator< int64_t > > __v2( __v1.begin(), __v1.end() );

	ASSERT_TRUE( __v1 == __v2 );
}


TEST( VECTOR_OEPRATOR, not_equal_operator ) {
	uint64_t                     __v_size = 1000000;
	std::unique_ptr< int64_t[] > __v_range( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v1( __v_range.get(), __v_range.get() + __v_size );
	nya::vector< int64_t, core::allocator< int64_t > > __v2( __v1.begin(), __v1.end() );

	ASSERT_FALSE( __v1 != __v2 );
}

TEST( VECTOR_OEPRATOR, less_than_operator ) {
	uint64_t                     __v_size = 1000000;
	std::unique_ptr< int64_t[] > __v_range1( new int64_t[ __v_size ]{} );
	std::unique_ptr< int64_t[] > __v_range2( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		auto rand       = distribution( generator );
		__v_range1[ i ] = rand;
		__v_range2[ i ] = rand + 1;
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v1( __v_range1.get(), __v_range1.get() + __v_size );
	nya::vector< int64_t, core::allocator< int64_t > > __v2( __v_range2.get(), __v_range2.get() + __v_size );

	ASSERT_TRUE( __v1 < __v2 );
}

TEST( VECTOR_OEPRATOR, less_equal_operator ) {
	uint64_t                     __v_size = 1000000;
	std::unique_ptr< int64_t[] > __v_range1( new int64_t[ __v_size ]{} );
	std::unique_ptr< int64_t[] > __v_range2( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		auto rand       = distribution( generator );
		__v_range1[ i ] = rand;
		__v_range2[ i ] = rand + 1;
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v1( __v_range1.get(), __v_range1.get() + __v_size );
	nya::vector< int64_t, core::allocator< int64_t > > __v2( __v_range2.get(), __v_range2.get() + __v_size );

	ASSERT_TRUE( __v1 < __v2 );
	ASSERT_TRUE( __v1 <= __v2 );
	ASSERT_FALSE( __v1 == __v2 );
}

TEST( VECTOR_OEPRATOR, greater_than_operator ) {
	uint64_t                     __v_size = 1000000;
	std::unique_ptr< int64_t[] > __v_range1( new int64_t[ __v_size ]{} );
	std::unique_ptr< int64_t[] > __v_range2( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		auto rand       = distribution( generator );
		__v_range1[ i ] = rand;
		__v_range2[ i ] = rand - 1;
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v1( __v_range1.get(), __v_range1.get() + __v_size );
	nya::vector< int64_t, core::allocator< int64_t > > __v2( __v_range2.get(), __v_range2.get() + __v_size );

	ASSERT_TRUE( __v1 > __v2 );
}

TEST( VECTOR_OEPRATOR, greater_equal_operator ) {
	uint64_t                     __v_size = 1000000;
	std::unique_ptr< int64_t[] > __v_range1( new int64_t[ __v_size ]{} );
	std::unique_ptr< int64_t[] > __v_range2( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		auto rand       = distribution( generator );
		__v_range1[ i ] = rand;
		__v_range2[ i ] = rand - 1;
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v1( __v_range1.get(), __v_range1.get() + __v_size );
	nya::vector< int64_t, core::allocator< int64_t > > __v2( __v_range2.get(), __v_range2.get() + __v_size );

	ASSERT_TRUE( __v1 > __v2 );
	ASSERT_TRUE( __v1 >= __v2 );
	ASSERT_FALSE( __v1 == __v2 );
}
