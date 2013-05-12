#ifndef __H_CODE_BLOCK_INTERFACE__
#define __H_CODE_BLOCK_INTERFACE__

#include <kraken/CodeBlockItemInterface.h>

namespace kraken
{
  class CodeBlockInterface
  {
    public:
      virtual ~CodeBlockInterface() = 0;

      virtual int get_item_count() = 0;
      virtual CodeBlockItemInterface* get_item_by_id(int id) = 0;
      virtual CodeBlockItemInterface* get_item_by_va(va_t virtualAddress) = 0;
  };
}

inline kraken::CodeBlockInterface::~CodeBlockInterface()
{
}

#endif
