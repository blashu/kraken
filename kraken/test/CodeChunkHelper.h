#ifndef __H_CODE_CHUNK_HELPER__
#define __H_CODE_CHUNK_HELPER__

#include "kraken/CodeChunk.h"

class CodeChunkHelper 
{
  public:
    static CodeChunk GenerateCodeChunk(const rva_t& firstRva, const rva_t& lastRva);

    static CodeChunk GenerateCodeChunkWithOneJump(const rva_t& firstRva, const rva_t& lastRva, const rva_t& JmpRva);
};

#endif
