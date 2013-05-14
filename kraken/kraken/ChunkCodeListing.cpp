#include "ChunkCodeListing.h"

using namespace std;
using namespace kraken;
using namespace kraken::internal;

ChunkCodeListing::ChunkCodeListing( std::shared_ptr<Disassembler> disassembler )
{
  _disassembler = disassembler;
  _itemCount = 0;

  vector<chunk_interval_t> chunkIntervals;

  for(auto begin = disassembler->begin(), end = disassembler->end();
      begin != end;
      ++begin)
  {
    _blocks.push_back(ChunkCodeBlock( this, &(*begin)) );

    _itemCount += _blocks.back().get_item_count();

    chunkIntervals.push_back(chunk_interval_t(begin->first_va(), begin->last_va(),
                                              _blocks.size() - 1));
  }

  _chunkMap = chunk_tree_t(chunkIntervals);
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
  if( 0 <= blockId && blockId < _blocks.size() )
  {
    return _blocks[blockId].get_item_by_id(itemId);
  }
  return NULL;
}

CodeBlockItemInterface* ChunkCodeListing::get_item_by_va(va_t virtualAddress)
{
  return NULL;
}

code_item_location_t ChunkCodeListing::get_item_location_by_va(va_t virtualAddress)
{
  vector<chunk_interval_t> foundIntervals;
  _chunkMap.findOverlapping(virtualAddress, virtualAddress + 1, foundIntervals);

  if( 0x00 == foundIntervals.size() )
  {
    return code_item_location_t(-1, -1);
  }

  int blockId = foundIntervals[0].value;
  int itemId = _blocks[blockId].get_item_id_by_va(virtualAddress);

  if( -1 != itemId )
  {
    return code_item_location_t(blockId, itemId);
  }
  return code_item_location_t(-1, -1);
}

int ChunkCodeListing::block_count()
{
  return _blocks.size();
}

int ChunkCodeListing::item_count()
{
  return _itemCount;
}
