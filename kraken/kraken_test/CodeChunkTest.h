#include "..\kraken\CodeChunk.h"

#include <gtest\gtest.h>

class CodeChunkTest : public :: testing::Test
{
  protected:

    static const int _baseRva = 100;

    static const int _lastRva = 200;

    CodeChunk _chunk;


    virtual void SetUp()
    {
      _chunk = GenerateCodeChunk( _baseRva, _lastRva );
    }

    CodeChunk GenerateCodeChunk(const rva_t& firstRva, const rva_t& lastRva);
};