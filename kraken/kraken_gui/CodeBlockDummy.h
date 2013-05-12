#ifndef __H_CODE_BLOCK_DUMMY__
#define __H_CODE_BLOCK_DUMMY__

#include <kraken/CodeBlockInterface.h>

#include "CodeBlockItemDummy.h"

using namespace std;
using namespace kraken;

class CodeBlockDummy : public CodeBlockInterface
{
  public:
    ~CodeBlockDummy() {}

    int get_item_count();
    CodeBlockItemInterface* get_item_by_id(int id);
    CodeBlockItemInterface* get_item_by_va(va_t virtualAddress);

  private:
    CodeBlockItemDummy _codeBlockItemDummy;
};


int CodeBlockDummy::get_item_count()
{
  return 10;
}

CodeBlockItemInterface* CodeBlockDummy::get_item_by_id(int id)
{
  return &_codeBlockItemDummy;
}

CodeBlockItemInterface* CodeBlockDummy::get_item_by_va(va_t virtualAddress)
{
  return &_codeBlockItemDummy;
}

#endif
