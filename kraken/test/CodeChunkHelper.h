#ifndef __H_CODE_CHUNK_HELPER__
#define __H_CODE_CHUNK_HELPER__

#include "kraken/CodeChunk.h"

class CodeChunkHelper 
{
  public:
    static CodeChunk GenerateCodeChunk(const va_t& firstVa, const va_t& lastVa);

    static CodeChunk GenerateCodeChunkWithOneJump(const va_t& firstVa, const va_t& lastVa, const va_t& JmpVa);
};

#endif
