#include "../kraken/Disassembler.h"
#include "MockDisassembler.h"
#include "CodeChunkHelper.h"

class DisassemblerTest : public :: testing::Test
{
  protected:
    MockDisassembler _disassembler;

    Disassembler _container;

    DisassemblerTest() : _container( _disassembler )
    {}
};
