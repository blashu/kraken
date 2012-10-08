#include "DisasmHelper.h"

#include <cstring>

bool DisasmHelper::cmp(const DISASM& first, const DISASM& second)
{
  return 0 == memcmp( &first, &second, sizeof(DISASM) );
}