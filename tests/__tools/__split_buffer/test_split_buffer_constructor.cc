#include "__memory/compress_pair.h"
#include "__split_buffer.h"
#include "gtest/gtest.h"

#include <cstddef>
#include <memory>
#include <random>
#include <type_traits>
#include <vector>

static core::random_device                       rd;
static core::mt19937                             generator( rd() );
static core::uniform_int_distribution< int64_t > distribution( 0 );

TEST( SPLIT_BUFFER_CONSTRUCTOR, default_construct ) {
	nya::__split_buffer< int, core::allocator< int > > __buffer{};
	EXPECT_EQ( nullptr, __buffer.__begin );
	EXPECT_EQ( nullptr, __buffer.__first );
	EXPECT_EQ( nullptr, __buffer.__end );
	EXPECT_EQ( nullptr, __buffer.__end_cap() );
	EXPECT_TRUE( noexcept( core::allocator< int >() ) );
}

TEST( SPLIT_BUFFER_CONSTRUCTOR, allocator_construct ) {
	core::allocator< int64_t >                             __a;
	nya::__split_buffer< int, core::allocator< int64_t > > __buffer( __a );
	EXPECT_EQ( nullptr, __buffer.__begin );
	EXPECT_EQ( nullptr, __buffer.__first );
	EXPECT_EQ( nullptr, __buffer.__end );
	EXPECT_EQ( nullptr, __buffer.__end_cap() );

	size_t alloc_size = 10000;
	auto   ptr        = __buffer.__alloc().allocate( alloc_size );
	ASSERT_TRUE( ptr != nullptr );
	for ( size_t i = 0; i < alloc_size; i++ ) {
		auto rand = distribution( generator );
		ptr[ i ]  = rand;
		EXPECT_EQ( rand, ptr[ i ] );
	}
	__buffer.__alloc().deallocate( ptr, alloc_size );
}

TEST( SPLIT_BUFFER_CONSTRUCTOR, const_allocator_construct ) {
	core::allocator< int64_t >                             __a;
	nya::__split_buffer< int, core::allocator< int64_t > > __buffer( __a );
	EXPECT_EQ( nullptr, __buffer.__begin );
	EXPECT_EQ( nullptr, __buffer.__first );
	EXPECT_EQ( nullptr, __buffer.__end );
	EXPECT_EQ( nullptr, __buffer.__end_cap() );

	size_t alloc_size = 10000;
	auto   ptr        = __buffer.__alloc().allocate( alloc_size );
	ASSERT_TRUE( ptr != nullptr );
	for ( size_t i = 0; i < alloc_size; i++ ) {
		auto rand = distribution( generator );
		ptr[ i ]  = rand;
		EXPECT_EQ( rand, ptr[ i ] );
	}
	__buffer.__alloc().deallocate( ptr, alloc_size );
}

TEST( SPLIT_BUFFER_CONSTRUCTOR, value_construct ) {
	size_t                                                     buffer_size = 1000000;
	size_t                                                     start_size  = 0;
	core::allocator< int64_t >                                 __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer( buffer_size, start_size, __a );

	ASSERT_TRUE( __buffer.__first != nullptr );
	EXPECT_EQ( __buffer.__begin, __buffer.__first + start_size );
	EXPECT_EQ( __buffer.__end, __buffer.__first + start_size );
	EXPECT_EQ( __buffer.__end_cap(), __buffer.__first + buffer_size );
	EXPECT_EQ( __buffer.__end_cap() - __buffer.__begin, buffer_size );

	for ( size_t i = 0; i < buffer_size; i++ ) {
		auto rand             = distribution( generator );
		__buffer.__first[ i ] = rand;
		EXPECT_TRUE( rand == __buffer.__first[ i ] );
	}
}

TEST( SPLIT_BUFFER_CONSTRUCTOR, move_construct ) {
	size_t                                                     buffer_size = 1000000;
	const core::allocator< int64_t >                           __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __moved_buffer( __a );

	EXPECT_EQ( nullptr, __moved_buffer.__begin );
	EXPECT_EQ( nullptr, __moved_buffer.__first );
	EXPECT_EQ( nullptr, __moved_buffer.__end );
	EXPECT_EQ( nullptr, __moved_buffer.__end_cap() );

	auto ptr = __moved_buffer.__alloc().allocate( buffer_size );

	__moved_buffer.__first     = ptr;
	__moved_buffer.__begin     = __moved_buffer.__first;
	__moved_buffer.__end       = __moved_buffer.__first + buffer_size;
	__moved_buffer.__end_cap() = __moved_buffer.__first + buffer_size;

	core::vector< int64_t > rands;
	for ( size_t i = 0; i < buffer_size; i++ ) {
		auto rand = distribution( generator );
		rands.push_back( rand );
		__moved_buffer.__first[ i ] = rand;
		EXPECT_TRUE( rand == __moved_buffer.__first[ i ] );
	}

	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer( core::move( __moved_buffer ) );

	ASSERT_TRUE( __buffer.__first != nullptr );
	ASSERT_TRUE( __buffer.__first == ptr );
	EXPECT_TRUE( __buffer.__begin != nullptr );
	EXPECT_EQ( __buffer.__begin, ptr );
	EXPECT_EQ( __buffer.__end, __buffer.__first + buffer_size );
	EXPECT_EQ( __buffer.__end_cap(), __buffer.__first + buffer_size );
	EXPECT_EQ( __buffer.__end_cap() - __buffer.__begin, buffer_size );

	for ( size_t i = 0; i < buffer_size; i++ ) {
		EXPECT_TRUE( rands[ i ] == __buffer.__first[ i ] );
	}
}

TEST( SPLIT_BUFFER_CONSTRUCTOR, move_construct_with_allocator ) {
	size_t                                                     buffer_size = 1000000;
	const core::allocator< int64_t >                           __a;
	nya::__split_buffer< int64_t, core::allocator< int64_t > > __moved_buffer( __a );

	EXPECT_EQ( nullptr, __moved_buffer.__begin );
	EXPECT_EQ( nullptr, __moved_buffer.__first );
	EXPECT_EQ( nullptr, __moved_buffer.__end );
	EXPECT_EQ( nullptr, __moved_buffer.__end_cap() );

	auto ptr = __moved_buffer.__alloc().allocate( buffer_size );

	__moved_buffer.__first     = ptr;
	__moved_buffer.__begin     = __moved_buffer.__first;
	__moved_buffer.__end       = __moved_buffer.__first + buffer_size;
	__moved_buffer.__end_cap() = __moved_buffer.__first + buffer_size;

	core::vector< int64_t > rands;
	for ( size_t i = 0; i < buffer_size; i++ ) {
		auto rand = distribution( generator );
		rands.push_back( rand );
		__moved_buffer.__first[ i ] = rand;
		EXPECT_TRUE( rand == __moved_buffer.__first[ i ] );
	}

	nya::__split_buffer< int64_t, core::allocator< int64_t > > __buffer_same_allocator( core::move( __moved_buffer ), __a );

	EXPECT_TRUE( __buffer_same_allocator.__first != nullptr );
	EXPECT_TRUE( __buffer_same_allocator.__first == ptr );
	EXPECT_TRUE( __buffer_same_allocator.__begin != nullptr );
	EXPECT_TRUE( __buffer_same_allocator.__begin == ptr );
	EXPECT_TRUE( __buffer_same_allocator.__end == __buffer_same_allocator.__begin + buffer_size );
	EXPECT_TRUE( __buffer_same_allocator.__end == __buffer_same_allocator.__first + buffer_size );
	EXPECT_TRUE( __buffer_same_allocator.__end_cap() == __buffer_same_allocator.__begin + buffer_size );
	EXPECT_TRUE( __buffer_same_allocator.__end_cap() == __buffer_same_allocator.__first + buffer_size );

	for ( size_t i = 0; i < buffer_size; i++ ) {
		EXPECT_EQ( rands[ i ], __buffer_same_allocator.__first[ i ] );
		EXPECT_EQ( rands[ i ], __buffer_same_allocator.__begin[ i ] );
	}
}

GTEST_API_ int main( int argc, char* argv[] ) {
	testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
