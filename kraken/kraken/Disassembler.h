#ifndef __H_DISASSMEBLER__
#define __H_DISASSMEBLER__

#include "kraken.h"
#include "AsmCode.h"
#include "CodeChunk.h"

using namespace kraken;

KRAKEN_API_ class Disassembler
{
  public:
    virtual int disassemble(AsmCode *asmCode) const = 0;

    virtual CodeChunk disassemble_code_chunk(rva_t instrAddr) const = 0;

    virtual rva_t entry_point() const = 0;
};

#endif