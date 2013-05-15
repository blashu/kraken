#include "ChunkCodeBlockItem.h"

#include "ChunkCodeListing.h"

using namespace kraken;
using namespace kraken::internal;

ChunkCodeBlockItem::ChunkCodeBlockItem(ChunkCodeListing* codeListing, const AsmCode* asmCode )
{
  _asmCode = asmCode;
  _codeListing = codeListing;
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
  return false == go_to().empty();
}

const vector<code_item_location_t>& ChunkCodeBlockItem::go_to()
{
  if( 0x00 != _goTo.size() )
  {
    return _goTo;
  }

  if( ( _asmCode->is_branch() ) &&
      ( 0x00 != _asmCode->Instruction.AddrValue ) )
  {
    _goTo.push_back( _codeListing->get_item_location_by_va( _asmCode->Instruction.AddrValue ) );
  }

  return _goTo;
}
