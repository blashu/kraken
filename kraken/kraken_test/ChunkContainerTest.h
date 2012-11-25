#include "..\kraken\ChunkContainer.h"
#include "MockDisassembler.h"
#include "CodeChunkHelper.h"

class ChunkContainerTest : public :: testing::Test
{
  protected:
    MockDisassembler _disassembler;

    ChunkContainer _container;
};