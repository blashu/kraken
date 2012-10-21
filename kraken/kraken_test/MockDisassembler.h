#include "..\kraken\Disassembler.h"

#include <gmock\gmock.h>

class MockDisassembler : public Disassembler
{
  public:
    MOCK_METHOD1(disassemble, int (AsmCode *disasmResult));
};