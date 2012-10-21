#ifndef __H_DISASSMEBLER__
#define __H_DISASSMEBLER__

#include "kraken.h"
#include "AsmCode.h"
#include "CodeChunk.h"

using namespace kraken;

KRAKEN_API_ class Disassembler
{
  public:


    virtual int disassemble(AsmCode *disasmResult) const = 0;

  private:
    // Calculate offset in the memory based on the passed virtual address
    //int rva_to_offset( const int &rva, unsigned char  );
};

#endif