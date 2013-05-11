#ifndef __H_CODE_LISTING_INTERFACE__
#define __H_CODE_LISTING_INTERFACE__

#include <kraken/CodeBlockInterface.h>

namespace kraken
{
  // Is used to represent various types of disassembled\decompiled code
  class CodeListingInterface
  {
    public:
      virtual CodeBlockItemInterface* get_item_by_id(int blockId, int itemId) = 0;
      virtual CodeBlockItemInterface* get_by_va(va_t virtualAddress) = 0;
      virtual CodeBlockInterface* get_block_by_id(int id) = 0;
      virtual int get_block_count() = 0;
      virtual int get_item_count() = 0;
  };
}

#endif
