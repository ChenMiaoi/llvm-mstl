#include "__split_buffer.h"
#include "gtest/gtest.h"

TEST( SPLIT_BUFFER_FUNCTIONS, size ) {
	size_t                                                     buffer_size = 100000;
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
	size_t                                                     buffer_size = 100000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer{
		buffer_size, start_size, __a };

	__buffer.clear();
}
