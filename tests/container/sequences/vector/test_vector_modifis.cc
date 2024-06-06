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

TEST( VECTOR_MODIFIES, clear ) {
	size_t                        __v_size = 1000000;
	core::unique_ptr< int64_t[] > __v_range( new int64_t[ __v_size ]{} );

	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v( __v_range.get(), __v_range.get() + __v_size );
	ASSERT_EQ( __v_size, __v.size() );
	ASSERT_EQ( __v_size, __v.capacity() );

	__v.clear();
	ASSERT_EQ( 0, __v.size() );
	ASSERT_EQ( __v_size, __v.capacity() );
}

TEST( VECTOR_MODIFIES, normal_insert ) {
	size_t                                    __v_size = 1000000;
	core::uniform_int_distribution< int64_t > __dis( 0, (int64_t) __v_size - 1 );
	core::unique_ptr< int64_t[] >             __v_range( new int64_t[ __v_size ]{} );

	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v( __v_range.get(), __v_range.get() + __v_size );
	ASSERT_EQ( __v_size, __v.size() );
	ASSERT_EQ( __v_size, __v.capacity() );

	int64_t __insert_var = distribution( generator );
	__v.insert( __v.begin(), __insert_var );
	ASSERT_EQ( __v_size + 1, __v.size() );
	ASSERT_EQ( __v_size * 2, __v.capacity() );
	ASSERT_EQ( __insert_var, __v[ 0 ] );

	__v.insert( __v.end(), __insert_var );
	ASSERT_EQ( __v_size + 2, __v.size() );
	ASSERT_EQ( __v_size * 2, __v.capacity() );
	ASSERT_EQ( __insert_var, __v[ __v_size + 1 ] );

	int64_t __insert_pos = __dis( generator );
	__v.insert( __v.begin() + __insert_pos, __insert_var );
	ASSERT_EQ( __v_size + 3, __v.size() );
	ASSERT_EQ( __v_size * 2, __v.capacity() );
	ASSERT_EQ( __insert_var, __v[ (uint64_t) __insert_pos ] );
}

TEST( VECTOR_MODIFIES, move_insert ) {
	size_t                                    __v_size = 1000000;
	core::uniform_int_distribution< int64_t > __dis( 0, (int64_t) __v_size - 1 );
	core::unique_ptr< int64_t[] >             __v_range( new int64_t[ __v_size ]{} );

	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v( __v_range.get(), __v_range.get() + __v_size );
	ASSERT_EQ( __v_size, __v.size() );
	ASSERT_EQ( __v_size, __v.capacity() );

	int64_t __insert_var = distribution( generator );
	__v.insert( __v.begin(), core::move( __insert_var ) );
	ASSERT_EQ( __v_size + 1, __v.size() );
	ASSERT_EQ( __v_size * 2, __v.capacity() );
	ASSERT_EQ( __insert_var, *( __v.begin().base() ) );

	__v.insert( __v.end(), core::move( __insert_var ) );
	ASSERT_EQ( __v_size + 2, __v.size() );
	ASSERT_EQ( __v_size * 2, __v.capacity() );
	ASSERT_EQ( __insert_var, *( ( __v.end() - 1 ).base() ) );

	int64_t __insert_pos = __dis( generator );
	__v.insert( __v.begin() + __insert_pos, core::move( __insert_var ) );
	ASSERT_EQ( __v_size + 3, __v.size() );
	ASSERT_EQ( __v_size * 2, __v.capacity() );
	ASSERT_EQ( __insert_var, __v[ (uint64_t) __insert_pos ] );

	__v.insert( __v.begin(), distribution( generator ) );
	ASSERT_EQ( __v_size + 4, __v.size() );
	ASSERT_EQ( __v_size * 2, __v.capacity() );
	ASSERT_LE( 0, *( __v.begin().base() ) );

	__v.insert( __v.end(), core::move( __insert_var ) );
	ASSERT_EQ( __v_size + 5, __v.size() );
	ASSERT_EQ( __v_size * 2, __v.capacity() );
	ASSERT_LE( 0, *( ( __v.end() - 1 ).base() ) );

	__insert_pos = __dis( generator );
	__v.insert( __v.begin() + __insert_pos, distribution( generator ) );
	ASSERT_EQ( __v_size + 6, __v.size() );
	ASSERT_EQ( __v_size * 2, __v.capacity() );
	ASSERT_LE( 0, __v[ (uint64_t) __insert_pos ] );
}

TEST( VECTOR_MODIFIES, copy_insert ) {
	size_t                                    __v_size = 1000000;
	core::uniform_int_distribution< int64_t > __dis( 0, (int64_t) __v_size - 1 );
	core::unique_ptr< int64_t[] >             __v_range( new int64_t[ __v_size ]{} );

	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v( __v_range.get(), __v_range.get() + __v_size );
	ASSERT_EQ( __v_size, __v.size() );
	ASSERT_EQ( __v_size, __v.capacity() );

	int64_t  __insert_var   = distribution( generator );
	uint64_t __insert_count = 500;
	__v.insert( __v.begin(), __insert_count, __insert_var );
	ASSERT_EQ( __v_size + __insert_count, __v.size() );
	ASSERT_EQ( __v_size * 2, __v.capacity() );
	for ( uint64_t i = 0; i < __insert_count; i++ ) {
		ASSERT_EQ( __insert_var, __v[ i ] );
	}

	__v.insert( __v.end(), __insert_count, __insert_var );
	ASSERT_EQ( __v_size + __insert_count * 2, __v.size() );
	ASSERT_EQ( __v_size * 2, __v.capacity() );
	ASSERT_EQ( __insert_var, *( ( __v.end() - 1 ).base() ) );
	for ( uint64_t i = 0; i < __insert_count; i++ ) {
		ASSERT_EQ( __insert_var, __v[ __v_size + __insert_count + i ] );
	}

	int64_t __insert_pos = __dis( generator );
	//! FATAL BUG
	__v.insert( __v.begin() + __insert_pos, __insert_count, __insert_var );
	ASSERT_EQ( __v_size + __insert_count * 3, __v.size() );
	ASSERT_EQ( __v_size * 2, __v.capacity() );
	ASSERT_EQ( __insert_var, *( ( __v.end() - 1 ).base() ) );
	for ( uint64_t i = 0; i < __v_size; i++ ) {
		ASSERT_EQ( __insert_var, __v[ (uint64_t) __insert_pos + i ] );
	}
}

TEST( VECTOR_MODIFIES, forward_insert ) {
	size_t                                    __v_size     = 1000000;
	size_t                                    __v_size_ins = 5000;
	core::uniform_int_distribution< int64_t > __dis( 0, (int64_t) __v_size - 1 );
	core::unique_ptr< int64_t[] >             __v_range1( new int64_t[ __v_size ]{} );
	core::unique_ptr< int64_t[] >             __v_range2( new int64_t[ __v_size ]{} );

	for ( size_t i = 0; i < __v_size; i++ ) {
		__v_range1[ i ] = distribution( generator );
	}
	for ( size_t i = 0; i < __v_size_ins; i++ ) {
		__v_range2[ i ] = distribution( generator );
	}

	nya::vector< int64_t, core::allocator< int64_t > > __v( __v_range1.get(), __v_range1.get() + __v_size );
	ASSERT_EQ( __v_size, __v.size() );
	ASSERT_EQ( __v_size, __v.capacity() );

	nya::vector< int64_t, core::allocator< int64_t > > __v_ins( __v_range2.get(), __v_range2.get() + __v_size_ins );
	ASSERT_EQ( __v_size_ins, __v_ins.size() );
	ASSERT_EQ( __v_size_ins, __v_ins.capacity() );

	__v.insert( __v.begin(), __v_ins.begin(), __v_ins.end() );
	ASSERT_EQ( __v_size + __v_size_ins, __v.size() );
	ASSERT_EQ( __v_size * 2, __v.capacity() );
	for ( size_t i = 0; i < __v_ins.size(); i++ ) {
		ASSERT_EQ( __v_ins[ i ], __v[ i ] );
	}

	__v.insert( __v.end(), __v_ins.begin(), __v_ins.end() );
	ASSERT_EQ( __v_size + __v_size_ins * 2, __v.size() );
	ASSERT_EQ( __v_size * 2, __v.capacity() );
	for ( size_t i = 0; i < __v_ins.size(); i++ ) {
		ASSERT_EQ( __v_ins[ i ], __v[ __v_size + i ] );
	}


	int64_t __insert_pos = __dis( generator );
	__v.insert( __v.begin() + __insert_pos, __v_ins.begin(), __v_ins.end() );
	ASSERT_EQ( __v_size + __v_size_ins * 3, __v.size() );
	ASSERT_EQ( __v_size * 2, __v.capacity() );
	for ( size_t i = 0; i < __v_ins.size(); i++ ) {
		ASSERT_EQ( __v_ins[ i ], __v[ (uint64_t) __insert_pos + i ] );
	}
}
