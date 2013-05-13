#include "ChunkCodeBlock.h"

using namespace kraken;
using namespace kraken::internal;

ChunkCodeBlock::ChunkCodeBlock( const CodeChunk* codeChunk )
{
  _codeChunk = codeChunk;

  for(auto begin = codeChunk->begin(), end = codeChunk->end();
      begin != end;
      ++begin)
  {
    items.push_back(ChunkCodeBlockItem(&(*begin)));
  }
}

ChunkCodeBlock::~ChunkCodeBlock()
{
}

CodeBlockItemInterface* ChunkCodeBlock::get_item_by_id(int id)
{
  if( ( 0 < id ) || ( id < items.size() ) )
  {
    return reinterpret_cast<CodeBlockItemInterface*>( &(items[id]) );
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


