#include "__split_buffer.h"
#include "gtest/gtest.h"

#include <memory>
#include <random>

static core::random_device                       rd;
static core::mt19937                             generator( rd() );
static core::uniform_int_distribution< int64_t > distribution( 0 );

TEST( SPLIT_BUFFER_ITERATOR, iterator_begin ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	ASSERT_TRUE( __buffer.begin() != nullptr );
	EXPECT_TRUE( __buffer.begin() == __buffer.__begin );
	EXPECT_TRUE( __buffer.begin() == __buffer.__end_cap() - buffer_size + start_size );
}

TEST( SPLIT_BUFFER_ITERATOR, const_iterator_begin ) {
	size_t                                                           buffer_size = 1000000;
	size_t                                                           start_size  = 0;
	core::allocator< int64_t >                                       __a;
	const nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	ASSERT_TRUE( __buffer.begin() != nullptr );
	EXPECT_TRUE( __buffer.begin() == __buffer.__begin );
	EXPECT_TRUE( __buffer.begin() == __buffer.__first + start_size );
	EXPECT_TRUE( __buffer.begin() == __buffer.__end_cap() - buffer_size + start_size );
}

TEST( SPLIT_BUFFER_ITERATOR, iterator_end ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	ASSERT_TRUE( __buffer.end() != nullptr );
	EXPECT_TRUE( __buffer.end() == __buffer.__end );
	EXPECT_TRUE( __buffer.end() == __buffer.__first + start_size );
}

TEST( SPLIT_BUFFER_ITERATOR, const_iterator_end ) {
	size_t                                                           buffer_size = 1000000;
	size_t                                                           start_size  = 0;
	core::allocator< int64_t >                                       __a;
	const nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	ASSERT_TRUE( __buffer.end() != nullptr );
	EXPECT_TRUE( __buffer.end() == __buffer.__end );
	EXPECT_TRUE( __buffer.end() == __buffer.__first + start_size );
}
