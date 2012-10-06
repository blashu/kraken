#include <gtest\gtest.h>

#include "..\kraken\CodeChunk.h"

TEST(CodeChunkTest, NotIncludedAddress) {
  CodeChunk chunk;

  EXPECT_EQ( false, chunk.is_address_included( 1000 ) );
}