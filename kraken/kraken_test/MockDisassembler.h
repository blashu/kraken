#include "..\kraken\Disassembler.h"

#include <gmock\gmock.h>

class MockDisassembler : public Disassembler
{
  public:
    MOCK_CONST_METHOD1(disassemble, int (AsmCode *disasmResult));
    MOCK_CONST_METHOD1(disassemble_code_chunk, CodeChunk (rva_t instrAddr));
    MOCK_CONST_METHOD0(entry_point, rva_t ());
};