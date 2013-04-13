#include "..\kraken\Decoder.h"

#include <gmock\gmock.h>

class MockDisassembler : public Decoder
{
  public:
    MOCK_CONST_METHOD1(decode, int (AsmCode *disasmResult));
    MOCK_CONST_METHOD1(decode_chunk, CodeChunk (rva_t instrAddr));
    MOCK_CONST_METHOD0(entry_point, rva_t ());
};
