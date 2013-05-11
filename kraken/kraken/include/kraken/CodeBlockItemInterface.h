#ifndef __H_CODE_BLOCK_ITEM_INTERFACE__
#define __H_CODE_BLOCK_ITEM_INTERFACE__

#include <string>
#include <vector>
#include <kraken/kraken.h>

using namespace std;

namespace kraken
{
  class CodeBlockItemInterface
  {
    public:
      virtual va_t get_va() = 0;
      virtual string to_string(const string& format = "") = 0;
      virtual bool is_branch() = 0;
      virtual const vector<va_t>& go_to() = 0;
  };
}

#endif
