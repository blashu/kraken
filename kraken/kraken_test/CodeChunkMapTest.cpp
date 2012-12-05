#include "CodeChunkMapTest.h"

TEST_F(CodeChunkMapTest, Add_AddTwoSequentualItems_SizeIsTwo)
{
  _map.add( CodeChunkMap::ChunkLocation( 0, 10 ) );
  _map.add( CodeChunkMap::ChunkLocation( 11, 20 ) );

  EXPECT_EQ( 2, _map.size() );
}

TEST_F(CodeChunkMapTest, Add_AddTwoIntersectionItems_SizeIsOne)
{
  _map.add( CodeChunkMap::ChunkLocation( 0, 10 ) );
  _map.add( CodeChunkMap::ChunkLocation( 10, 20 ) );

  EXPECT_EQ( 1, _map.size() );
}

TEST_F(CodeChunkMapTest, Find_AdressIsIncluded_IncludesReturnTrue)
{
  _map.add( CodeChunkMap::ChunkLocation( 1, 10 ) );

  EXPECT_EQ( true, _map.includes( 1 ) );
  EXPECT_EQ( true, _map.includes( 4 ) );
  EXPECT_EQ( true, _map.includes( 10 ) );
}

TEST_F(CodeChunkMapTest, Find_AdressIsNotIncluded_IncludesReturnFalse)
{
  _map.add( CodeChunkMap::ChunkLocation( 1, 10 ) );

  EXPECT_EQ( false, _map.includes( 0 ) );
  EXPECT_EQ( false, _map.includes( 11 ) );
}
