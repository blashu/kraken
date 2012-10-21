#include "MockChunkContainer.h"
#include "MockDisassembler.h"

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
/*
TEST(DISABLED_ChunkContainerTest, Fill_WorksFine)
{
  MockChunkContainer container;
  MockDisassembler disassm;

  EXPECT_CALL( container, disassemble_code_chunk( _, _ ) ).Times(AtLeast(1));

  unsigned char buff[] = { 1,2,3 };
  vector<unsigned char> memBuf( buff, &(buff[ sizeof(buff)/sizeof(*buff) ]) );
  container.fill( memBuf, 0, 0 );
}*/