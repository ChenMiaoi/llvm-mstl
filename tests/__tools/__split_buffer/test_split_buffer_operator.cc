#include "__split_buffer.h"
#include "gtest/gtest.h"

#include <gtest/gtest.h>
#include <memory>
#include <random>
#include <utility>

static core::random_device                       rd;
static core::mt19937                             generator( rd() );
static core::uniform_int_distribution< int64_t > distribution( 0 );

TEST( SPLIT_BUFFER_OPERATOR, move_operator ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __swap_buffer{
		buffer_size, start_size, __a };

	core::vector< int64_t > rands;
	for ( size_t i = 0; i < buffer_size; i++ ) {
		auto rand = distribution( generator );
		rands.push_back( rand );
		__swap_buffer.push_back( rand );
	}

	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer = core::move( __swap_buffer );
	EXPECT_TRUE( __buffer.begin() != nullptr );
	EXPECT_TRUE( __buffer.end() != nullptr );
	EXPECT_TRUE( __buffer.__end_cap() != nullptr );

	EXPECT_TRUE( buffer_size == __buffer.size() );
	EXPECT_TRUE( buffer_size == __buffer.capacity() );

	for ( size_t i = 0; i < buffer_size - 1; i++ ) {
		__buffer.pop_back();
		EXPECT_TRUE( __buffer.back() == rands[ buffer_size - i - 2 ] );
	}
	EXPECT_TRUE( __buffer.back() == rands[ 0 ] );
	EXPECT_TRUE( 1 == __buffer.size() );
	EXPECT_TRUE( buffer_size == __buffer.capacity() );

	__buffer.pop_back();
	EXPECT_TRUE( 0 == __buffer.size() );
	EXPECT_TRUE( buffer_size == __buffer.capacity() );
}
