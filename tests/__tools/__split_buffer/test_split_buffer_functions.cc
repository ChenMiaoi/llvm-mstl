#include "__split_buffer.h"
#include "gtest/gtest.h"

#include <algorithm>
#include <memory>
#include <random>
#include <utility>
#include <vector>

static core::random_device                       rd;
static core::mt19937                             generator( rd() );
static core::uniform_int_distribution< int64_t > distribution( 0 );

TEST( SPLIT_BUFFER_FUNCTIONS, size ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	EXPECT_EQ( 0, __buffer.size() );

	__buffer.push_back( 1 );
	EXPECT_EQ( 1, __buffer.size() );

	__buffer.push_back( 2 );
	__buffer.push_back( 3 );
	EXPECT_EQ( 3, __buffer.size() );

	__buffer.push_back( 4 );
	__buffer.push_back( 5 );
	__buffer.push_back( 6 );
	EXPECT_EQ( 6, __buffer.size() );

	__buffer.push_back( 7 );
	__buffer.push_back( 8 );
	__buffer.push_back( 9 );
	__buffer.push_back( 10 );
	EXPECT_EQ( 10, __buffer.size() );

	EXPECT_EQ( 10, __buffer.end() - __buffer.begin() );
}

TEST( SPLIT_BUFFER_FUNCTIONS, clear ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	__buffer.clear();
}

TEST( SPLIT_BUFFER_FUNCTIONS, capacity ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	EXPECT_EQ( 0, __buffer.size() );
	EXPECT_EQ( buffer_size, __buffer.capacity() );

	__buffer.push_back( 1 );
	EXPECT_EQ( 1, __buffer.size() );
	EXPECT_EQ( buffer_size, __buffer.capacity() );

	__buffer.push_back( 2 );
	__buffer.push_back( 3 );
	EXPECT_EQ( 3, __buffer.size() );
	EXPECT_EQ( buffer_size, __buffer.capacity() );

	__buffer.push_back( 4 );
	__buffer.push_back( 5 );
	__buffer.push_back( 6 );
	EXPECT_EQ( 6, __buffer.size() );
	EXPECT_EQ( buffer_size, __buffer.capacity() );

	__buffer.push_back( 7 );
	__buffer.push_back( 8 );
	__buffer.push_back( 9 );
	__buffer.push_back( 10 );
	EXPECT_EQ( 10, __buffer.size() );
	EXPECT_EQ( buffer_size, __buffer.capacity() );

	ASSERT_TRUE( (size_t) ( __buffer.__end_cap() - __buffer.begin() ) == buffer_size );
}

TEST( SPLIT_BUFFER_FUNCTIONS, __first_spare ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	EXPECT_EQ( 0, __buffer.size() );
	EXPECT_EQ( buffer_size, __buffer.capacity() );

	ASSERT_EQ( 0, __buffer.begin() - __buffer.__first );
	EXPECT_EQ( 0, __buffer.__front_spare() );

	for ( size_t i = 0; i < buffer_size; i++ ) {
		__buffer.push_back( distribution( generator ) );
	}
	EXPECT_EQ( buffer_size, __buffer.size() );
	EXPECT_EQ( buffer_size, __buffer.capacity() );

	ASSERT_EQ( 0, __buffer.begin() - __buffer.__first );
	EXPECT_EQ( 0, __buffer.__front_spare() );

	start_size = 100;

	nya::__split_buffer< int64_t, core::allocator< int64_t > > __start_buffer{
		buffer_size, start_size, __a };

	EXPECT_EQ( 0, __start_buffer.size() );
	EXPECT_EQ( buffer_size, __start_buffer.capacity() );

	ASSERT_EQ( 100, __start_buffer.begin() - __start_buffer.__first );
	EXPECT_EQ( 100, __start_buffer.__front_spare() );

	for ( size_t i = 0; i < buffer_size - 100; i++ ) {
		__start_buffer.push_back( distribution( generator ) );
	}
	EXPECT_EQ( buffer_size - 100, __start_buffer.size() );
	EXPECT_EQ( buffer_size, __start_buffer.capacity() );

	ASSERT_EQ( 100, __start_buffer.begin() - __start_buffer.__first );
	EXPECT_EQ( 100, __start_buffer.__front_spare() );
}

TEST( SPLIT_BUFFER_FUNCTIONS, __back_spare ) {
	size_t                                                     buffer_size = 100000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	for ( size_t i = 0; i < buffer_size; i++ ) {
		auto rand = distribution( generator );
		__buffer.push_back( rand );
		EXPECT_EQ( buffer_size - i - 1, __buffer.__back_spare() );
	}
	EXPECT_EQ( 0, __buffer.__back_spare() );
}

TEST( SPLIT_BUFFER_FUNCTIONS, front ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	core::vector< int64_t > rands;
	for ( size_t i = 0; i < buffer_size; i++ ) {
		auto rand = distribution( generator );
		rands.push_back( rand );
		__buffer.push_back( rand );
	}
	EXPECT_EQ( __buffer.front(), rands.front() );
}

TEST( SPLIT_BUFFER_FUNCTIONS, const_front ) {
	size_t                                                           buffer_size = 1000000;
	size_t                                                           start_size  = 0;
	core::allocator< int64_t >                                       __a;
	const nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	*__buffer.__begin = 0;
	EXPECT_EQ( __buffer.front(), 0 );
}

TEST( SPLIT_BUFFER_FUNCTIONS, back ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	core::vector< int64_t > rands;
	for ( size_t i = 0; i < buffer_size; i++ ) {
		auto rand = distribution( generator );
		rands.push_back( rand );
		__buffer.push_back( rand );
	}
	EXPECT_EQ( __buffer.back(), rands.back() );
}

TEST( SPLIT_BUFFER_FUNCTIONS, reserve ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	EXPECT_EQ( 0, __buffer.size() );
	EXPECT_EQ( 0, __buffer.__end - __buffer.__begin );
	EXPECT_EQ( buffer_size, __buffer.capacity() );

	size_t reserve_size = 10000;

	__buffer.reserve( reserve_size );
	EXPECT_EQ( 0, __buffer.size() );
	EXPECT_EQ( 0, __buffer.__end - __buffer.__begin );
	EXPECT_EQ( reserve_size, __buffer.capacity() );

	__buffer.reserve( reserve_size * 10 );
	EXPECT_EQ( 0, __buffer.size() );
	EXPECT_EQ( 0, __buffer.__end - __buffer.__begin );
	EXPECT_EQ( reserve_size, __buffer.capacity() );

	__buffer.reserve( reserve_size / 10 );
	EXPECT_EQ( 0, __buffer.size() );
	EXPECT_EQ( 0, __buffer.__end - __buffer.__begin );
	EXPECT_EQ( reserve_size / 10, __buffer.capacity() );
}

TEST( SPLIT_BUFFER_FUNCTIONS, shrink_to_fit ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	size_t pushed_size = 100000;
	for ( size_t i = 0; i < pushed_size; i++ ) {
		__buffer.push_back( distribution( generator ) );
	}

	EXPECT_EQ( pushed_size, __buffer.size() );
	EXPECT_EQ( buffer_size, __buffer.capacity() );
	__buffer.shrink_to_fit();
	EXPECT_EQ( pushed_size, __buffer.size() );
	EXPECT_EQ( pushed_size, __buffer.capacity() );
}

TEST( SPLIT_BUFFER_FUNCTIONS, value_push_front ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	core::vector< int64_t > rands;
	for ( size_t i = 0; i < buffer_size; i++ ) {
		auto rand = distribution( generator );
		rands.push_back( rand );
		__buffer.push_front( rand );
	}

	EXPECT_TRUE( __buffer.size() == buffer_size );
	EXPECT_TRUE( __buffer.capacity() == buffer_size );

	core::reverse( rands.begin(), rands.end() );
	for ( size_t i = 0; i < buffer_size; i++ ) {
		EXPECT_TRUE( rands[ i ] == __buffer.begin()[ i ] );
	}
}

TEST( SPLIT_BUFFER_FUNCTIONS, move_push_front ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	core::vector< int64_t > rands;
	for ( size_t i = 0; i < buffer_size; i++ ) {
		auto rand = distribution( generator );
		rands.push_back( rand );
		__buffer.push_front( core::move( rand ) );
	}

	EXPECT_TRUE( __buffer.size() == buffer_size );
	EXPECT_TRUE( __buffer.capacity() == buffer_size );

	core::reverse( rands.begin(), rands.end() );
	for ( size_t i = 0; i < buffer_size; i++ ) {
		EXPECT_TRUE( rands[ i ] == __buffer.begin()[ i ] );
	}
}

TEST( SPLIT_BUFFER_FUNCTIONS, value_push_back ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	core::vector< int64_t > rands;
	for ( size_t i = 0; i < buffer_size; i++ ) {
		auto rand = distribution( generator );
		rands.push_back( rand );
		__buffer.push_back( rand );
	}

	EXPECT_TRUE( __buffer.size() == buffer_size );
	EXPECT_TRUE( __buffer.capacity() == buffer_size );

	for ( size_t i = 0; i < buffer_size; i++ ) {
		EXPECT_TRUE( rands[ i ] == __buffer.begin()[ i ] );
	}
}

TEST( SPLIT_BUFFER_FUNCTIONS, move_push_back ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	core::vector< int64_t > rands;
	for ( size_t i = 0; i < buffer_size; i++ ) {
		auto rand = distribution( generator );
		rands.push_back( rand );
		__buffer.push_back( core::move( rand ) );
	}

	EXPECT_TRUE( __buffer.size() == buffer_size );
	EXPECT_TRUE( __buffer.capacity() == buffer_size );

	for ( size_t i = 0; i < buffer_size; i++ ) {
		EXPECT_TRUE( rands[ i ] == __buffer.begin()[ i ] );
	}
}

TEST( SPLIT_BUFFER_FUNCTIONS, emplace_back ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	core::vector< int64_t > rands;
	for ( size_t i = 0; i < buffer_size; i++ ) {
		auto rand = distribution( generator );
		rands.push_back( rand );
		__buffer.emplace_back( rand );
	}

	EXPECT_TRUE( __buffer.size() == buffer_size );
	EXPECT_TRUE( __buffer.capacity() == buffer_size );

	for ( size_t i = 0; i < buffer_size; i++ ) {
		EXPECT_TRUE( rands[ i ] == __buffer.begin()[ i ] );
	}
}

TEST( SPLIT_BUFFER_FUNCTIONS, pop_front ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	core::vector< int64_t > rands;
	for ( size_t i = 0; i < buffer_size; i++ ) {
		auto rand = distribution( generator );
		rands.push_back( rand );
		__buffer.push_back( rand );
	}

	EXPECT_TRUE( __buffer.size() == buffer_size );
	EXPECT_TRUE( __buffer.capacity() == buffer_size );

	for ( size_t i = 0; i < buffer_size - 1; i++ ) {
		__buffer.pop_front();
		EXPECT_TRUE( __buffer.front() == rands[ i + 1 ] );
	}
	EXPECT_TRUE( __buffer.front() == rands[ buffer_size - 1 ] );
	EXPECT_TRUE( 1 == __buffer.size() );
	EXPECT_TRUE( buffer_size == __buffer.capacity() );

	__buffer.pop_front();
	EXPECT_TRUE( 0 == __buffer.size() );
	EXPECT_TRUE( buffer_size == __buffer.capacity() );
}

TEST( SPLIT_BUFFER_FUNCTIONS, pop_back ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	core::vector< int64_t > rands;
	for ( size_t i = 0; i < buffer_size; i++ ) {
		auto rand = distribution( generator );
		rands.push_back( rand );
		__buffer.push_back( rand );
	}

	EXPECT_TRUE( __buffer.size() == buffer_size );
	EXPECT_TRUE( __buffer.capacity() == buffer_size );

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

TEST( SPLIT_BUFFER_FUNCTIONS, swap ) {
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

	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer;
	__buffer.swap( __swap_buffer );

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
