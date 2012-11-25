#include "CodeChunkHelper.h"

CodeChunk CodeChunkHelper::GenerateCodeChunk(const rva_t& firstRva, const rva_t& lastRva)
{
  CodeChunk chunk;
  
  AsmCode tempDisasm;
  for( auto passedChunkBaseRva = firstRva; passedChunkBaseRva <= lastRva; ++passedChunkBaseRva )
  {
    tempDisasm.VirtualAddr = passedChunkBaseRva;
    chunk.add_to_chunk( tempDisasm );
  }
  
  return chunk;
}

CodeChunk CodeChunkHelper::GenerateCodeChunkWithOneJump(const rva_t& firstRva, const rva_t& lastRva, const rva_t& JmpRva)
{
  CodeChunk chunk;
  
  AsmCode tempDisasm;
  for( auto passedChunkBaseRva = firstRva; passedChunkBaseRva <= lastRva; ++passedChunkBaseRva )
  {
    if ( passedChunkBaseRva == JmpRva )
    {
      tempDisasm.Instruction.BranchType = kraken::JmpType;
      tempDisasm.Instruction.AddrValue = JmpRva;
    }
    else
    {
      tempDisasm.Instruction.BranchType = kraken::Undefined;
      tempDisasm.Instruction.AddrValue = -1;
    }

    tempDisasm.VirtualAddr = passedChunkBaseRva;
    chunk.add_to_chunk( tempDisasm );
  }
  
  return chunk;
}