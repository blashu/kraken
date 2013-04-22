#ifndef __H_DECODER__
#define __H_DECODER__

#include "kraken.h"
#include "CodeChunk.h"

using namespace kraken;

KRAKEN_API_ class Decoder
{
  public:
    virtual int decode(AsmCode *asmCode) const = 0;

    virtual CodeChunk decode_chunk(rva_t instrAddr) const = 0;

    virtual rva_t entry_point() const = 0;
};

#endif
