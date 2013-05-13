#include "ChunkCodeListing.h"

using namespace kraken;
using namespace kraken::internal;

ChunkCodeListing::ChunkCodeListing( std::shared_ptr<Disassembler> disassembler )
{
  _disassembler = disassembler;

  for(auto begin = disassembler->begin(), end = disassembler->end();
      begin != end;
      ++begin)
  {
    _blocks.push_back(&(*begin));
  }
}

ChunkCodeListing::~ChunkCodeListing()
{
}

CodeBlockInterface* ChunkCodeListing::get_block_by_id(int id)
{
  return &(_blocks[id]);
}

CodeBlockItemInterface* ChunkCodeListing::get_item_by_id(int blockId, int itemId)
{
  return _blocks[blockId].get_item_by_id(itemId);
}

CodeBlockItemInterface* ChunkCodeListing::get_item_by_va(va_t virtualAddress)
{
  return NULL;
}

int ChunkCodeListing::block_count()
{
  return _blocks.size();
}

int ChunkCodeListing::item_count()
{
  return 0;
}
