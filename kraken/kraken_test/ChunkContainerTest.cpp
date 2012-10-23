#include "MockChunkContainer.h"
#include "MockDisassembler.h"

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;

TEST(DISABLED_ChunkContainerTest, Fill_StoresOneChunkCorrectly)
{
  MockDisassembler disasm;
  ChunkContainer container;

  CodeChunk chunk;

  AsmCode tempAsmCode;
  for( auto passedChunkBaseRva = 100; passedChunkBaseRva <= 200; ++passedChunkBaseRva )
  {
    tempAsmCode.VirtualAddr = passedChunkBaseRva;
    chunk.add_to_chunk( tempAsmCode );
  }

  EXPECT_CALL( disasm, entry_point()).Times(1).WillOnce(Return(0));
  EXPECT_CALL( disasm, disassemble_code_chunk( _ ) ).Times(AtLeast(1)).WillOnce(Return(chunk));

  container.fill( disasm );

  // Надо замутить проверку сохраненного результата.
  auto comparationResult = memcmp( &chunk, &container.begin(), sizeof( CodeChunk ) );
  EXPECT_EQ( 0, comparationResult );
}