#include "Disassembler.h"

using namespace kraken;

int Disassembler::disassemble(AsmCode *asmCode) const
{
  memset( asmCode, 0, sizeof( AsmCode ) );

  return 0;
}

CodeChunk Disassembler::disassemble_code_chunk(rva_t instrAddr) const
{
  CodeChunk codeChunk;

  return codeChunk;
}

rva_t Disassembler::entry_point() const
{
  return 0;
}