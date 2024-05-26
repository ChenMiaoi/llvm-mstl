#include "__split_buffer.h"
#include "gtest/gtest.h"

#include <gtest/gtest.h>
#include <memory>
#include <random>

static core::random_device                       rd;
static core::mt19937                             generator( rd() );
static core::uniform_int_distribution< int64_t > distribution( 0 );

TEST( SPLIT_BUFFER_HELPER, hello_world ) {
	// TODO
	EXPECT_EQ( "hello world", "hello world" );
}
