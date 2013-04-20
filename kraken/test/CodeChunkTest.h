#ifndef __H_CODE_CHUNK_TEST__
#define __H_CODE_CHUNK_TEST__

#include "..\kraken\CodeChunk.h"

#include <gtest\gtest.h>
#include "CodeChunkHelper.h"

class CodeChunkTest : public :: testing::Test
{
  protected:

    static const int _baseRva = 100;

    static const int _lastRva = 200;

    CodeChunk _chunk;
    
    virtual void SetUp()
    {
      _chunk = CodeChunkHelper::GenerateCodeChunk( _baseRva, _lastRva );
    }
};

#endif
