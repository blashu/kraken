#include "..\kraken\CodeChunk.h"

static class CodeChunkHelper 
{
  public:
    static CodeChunk GenerateCodeChunk(const rva_t& firstRva, const rva_t& lastRva);

    static CodeChunk GenerateCodeChunkWithOneJump(const rva_t& firstRva, const rva_t& lastRva, const rva_t& JmpRva);
};