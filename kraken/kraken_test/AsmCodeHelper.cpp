#include "AsmCodeHelper.h"

#include <cstring>

using namespace kraken;

bool AsmCodeHelper::cmp(const AsmCode& first, const AsmCode& second)
{
  return 0 == memcmp( &first, &second, sizeof(AsmCode) );
}