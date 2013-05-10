#include "DisassemblerTest.h"

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;

TEST_F(DisassemblerTest, Fill_Always_StartsDisasmingFromEntryPoint)
{
  const va_t start_va = 100;

  CodeChunk chunk = CodeChunkHelper::GenerateCodeChunk( 100, 200 );
  
  EXPECT_CALL( _disassembler, entry_point() ).Times( 1 ).WillOnce( Return( start_va ) );
  EXPECT_CALL( _disassembler, decode_chunk( start_va ) ).Times( AtLeast( 1 ) ).WillOnce( Return( chunk ) );

  _container.fill( _disassembler );

  EXPECT_EQ( chunk, _container.front() );
}

TEST_F(DisassemblerTest, Fill_Always_StoresOneChunkCorrectly)
{
  CodeChunk chunk = CodeChunkHelper::GenerateCodeChunk( 100, 200 );
  
  EXPECT_CALL( _disassembler, entry_point() ).Times( 1 ).WillOnce( Return( 0 ) );
  EXPECT_CALL( _disassembler, decode_chunk( _ ) ).Times( AtLeast( 1 ) ).WillOnce( Return( chunk ) );

  _container.fill( _disassembler );

  EXPECT_EQ( chunk, _container.front() );
}

TEST_F(DisassemblerTest, Fill_DisassembledChunksIntersect_MergesThem)
{
  CodeChunk chunk1 = CodeChunkHelper::GenerateCodeChunkWithOneJump( 100, 200, 150 );
  CodeChunk chunk2 = CodeChunkHelper::GenerateCodeChunk( 150, 250 );

  EXPECT_CALL( _disassembler, entry_point() ).Times( 1 ).WillOnce( Return( 0 ) );
  EXPECT_CALL( _disassembler, decode_chunk( _ ) ).Times( AtLeast( 2 ) ).WillOnce( Return( chunk1 ) ).WillOnce( Return ( chunk2 ) );
  
  _container.fill( _disassembler );

  EXPECT_EQ( chunk1.front(), _container.front().front() );
  EXPECT_EQ( chunk2.back(), _container.back().back() );
}
