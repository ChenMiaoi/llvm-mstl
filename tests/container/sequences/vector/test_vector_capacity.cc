//
// Created by dezi on 5/31/2024.
//
#include "__iterator/wrap_iter.h"
#include "vector.hpp"
#include "gtest/gtest.h"

#include <algorithm>
#include <gtest/gtest.h>
#include <initializer_list>
#include <limits>
#include <memory>
#include <random>
#include <stdint.h>

static core::random_device                       rd;
static core::mt19937                             generator( rd() );
static core::uniform_int_distribution< int64_t > distribution( 0, core::numeric_limits< int64_t >::max() - 1 );

TEST( VECTOR_CAPACITY, empty ) {
	nya::vector< int64_t, core::allocator< int64_t > > __v;
	ASSERT_TRUE( true == __v.empty() );
}

TEST( VECTOR_CAPACITY, size ) {
	uint64_t                     __v_size = 1000000;
	std::unique_ptr< int64_t[] > __v_range( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v( __v_range.get(), __v_range.get() + __v_size );

	ASSERT_TRUE( __v_size == __v.size() );
}

TEST( VECTOR_CAPACITY, max_size ) {
	uint64_t                     __v_size = 1000000;
	std::unique_ptr< int64_t[] > __v_range( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v( __v_range.get(), __v_range.get() + __v_size );

	ASSERT_TRUE( __v_size == __v.size() );
	ASSERT_FALSE( __v.size() == __v.max_size() );
}

TEST( VECTOR_CAPACITY, reserve ) {
	uint64_t __v_size = 1000000;

	nya::vector< int64_t, core::allocator< int64_t > > __v;
	__v.reserve( __v_size );

	ASSERT_TRUE( 0 == __v.size() );
	ASSERT_EQ( __v_size, __v.capacity() );
}

TEST( VECTOR_CAPACITY, capacity ) {
	uint64_t                     __v_size = 1000000;
	std::unique_ptr< int64_t[] > __v_range( new int64_t[ __v_size ]{} );
	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v( __v_range.get(), __v_range.get() + __v_size );

	ASSERT_TRUE( __v_size == __v.size() );
	ASSERT_TRUE( __v_size == __v.capacity() );
}

TEST( VECTOR_CAPACITY, shrink_to_fit ) {
	uint64_t __v_size = 1000000;

	nya::vector< int64_t, core::allocator< int64_t > > __v;
	__v.reserve( __v_size );
	ASSERT_EQ( __v_size, __v.capacity() );
	ASSERT_EQ( 0, __v.size() );

	for ( size_t i = 0; i < __v_size / 2; i++ ) {
		__v.emplace_back( distribution( generator ) );
	}

	ASSERT_TRUE( __v_size / 2 == __v.size() );
	ASSERT_TRUE( __v_size == __v.capacity() );

	__v.shrink_to_fit();
	ASSERT_TRUE( __v_size / 2 == __v.size() );
	ASSERT_TRUE( __v_size / 2 == __v.capacity() );
}
