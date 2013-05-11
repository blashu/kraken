#ifndef __H_CODE_LISTING_DUMMY__
#define __H_CODE_LISTING_DUMMY__

#include <kraken/CodeListingInterface.h>

#include "CodeBlockDummy.h"

using namespace std;
using namespace kraken;

class CodeListingDummy : public CodeListingInterface
{
  public:
    ~CodeListingDummy() {}

    CodeBlockItemInterface* get_item_by_id(int blockId, int itemId);
    CodeBlockItemInterface* get_item_by_va(va_t virtualAddress);
    CodeBlockInterface* get_block_by_id(int id);
    int block_count();
    int item_count();

  private:
    CodeBlockItemDummy _codeBlockItemDummy;
    CodeBlockDummy _codeBlockDummy;
};

CodeBlockItemInterface* CodeListingDummy::get_item_by_id(int blockId, int itemId)
{
  return &_codeBlockItemDummy;
}

CodeBlockItemInterface* CodeListingDummy::get_item_by_va(va_t virtualAddress)
{
  return &_codeBlockItemDummy;
}

CodeBlockInterface* CodeListingDummy::get_block_by_id(int id)
{
  return &_codeBlockDummy;
}

int CodeListingDummy::block_count()
{
  return 10;
}

int CodeListingDummy::item_count()
{
  return 100;
}

#endif
