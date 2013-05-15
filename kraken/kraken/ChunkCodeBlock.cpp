#include "ChunkCodeBlock.h"

#include <algorithm>

using namespace kraken;
using namespace kraken::internal;

ChunkCodeBlock::ChunkCodeBlock(ChunkCodeListing* codeListing, const CodeChunk* codeChunk )
{
  _codeChunk = codeChunk;

  for(auto it = codeChunk->begin(), end = codeChunk->end();
      it != end;
      ++it)
  {
    _items.push_back(ChunkCodeBlockItem(codeListing, &(*it)));
  }
}

ChunkCodeBlock::~ChunkCodeBlock()
{
}

CodeBlockItemInterface* ChunkCodeBlock::get_item_by_id(int id)
{
  if( ( 0 < id ) || ( id < _items.size() ) )
  {
    return reinterpret_cast<CodeBlockItemInterface*>( &(_items[id]) );
  }
  return NULL;
}

CodeBlockItemInterface* ChunkCodeBlock::get_item_by_va(va_t virtualAddress)
{
  return NULL;
}

int ChunkCodeBlock::get_item_count()
{
  return _codeChunk->size();
}

int ChunkCodeBlock::get_item_id_by_va(va_t virtualAddress)
{
  auto itemsSize = _items.size();
  if( 0x00 == itemsSize )
  {
    return -1;
  }

  if( 0x01 == itemsSize )
  {
    if( virtualAddress == _items[0].va() )
    {
      return 0;
    }
    return -1;
  }

  int left = 0;
  int right = itemsSize - 1;
  while( left < right )
  {
    int center = left + ( right - left ) / 2;
    va_t centerVa = _items[center].va();

    if( virtualAddress <= centerVa )
    {
      right = center;
    }
    else
    {
      left = center + 1;
    }
  }

  if( _items[right].va() == virtualAddress )
  {
    return right;
  }
  return -1;
}


