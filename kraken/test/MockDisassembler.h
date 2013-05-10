#include "kraken/Decoder.h"

#include <gmock\gmock.h>

class MockDisassembler : public Decoder
{
  public:
    MOCK_CONST_METHOD2(decode, int (va_t instrVirtualAddr, AsmCode *asmCode));
    MOCK_CONST_METHOD0(entry_point, va_t ());
};
