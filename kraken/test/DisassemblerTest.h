#ifndef __H_DISASSEMBLER_TEST__
#define __H_DISASSEMBLER_TEST__

#include "kraken/Disassembler.h"
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

#endif
