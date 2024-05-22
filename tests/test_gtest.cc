#include "gtest/gtest.h"

TEST( HelloTest, BasicAssertions ) {
	// Expect two strings not to be equal.
	EXPECT_STRNE( "hello", "world" );
	// Expect equality.
	EXPECT_EQ( 7 * 6, 42 );
}

GTEST_API_ int main( int argc, char* argv[] ) {
	testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}