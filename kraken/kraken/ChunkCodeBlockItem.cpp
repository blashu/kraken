#include "ChunkCodeBlockItem.h"

using namespace kraken;
using namespace kraken::internal;

ChunkCodeBlockItem::ChunkCodeBlockItem(const AsmCode* asmCode)
{
  _asmCode = asmCode;
}

ChunkCodeBlockItem::~ChunkCodeBlockItem()
{
}

va_t ChunkCodeBlockItem::va()
{
  return _asmCode->VirtualAddr;
}

string ChunkCodeBlockItem::to_string(const string& format)
{
  return _asmCode->CompleteInstr;
}

bool ChunkCodeBlockItem::is_branch()
{
  return 0x00 != goTo.size();
}

const vector<codeItemLocation_t>& ChunkCodeBlockItem::go_to()
{
  return goTo;
}
