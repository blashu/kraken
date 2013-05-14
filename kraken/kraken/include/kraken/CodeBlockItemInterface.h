#ifndef __H_CODE_BLOCK_ITEM_INTERFACE__
#define __H_CODE_BLOCK_ITEM_INTERFACE__

#include <string>
#include <vector>
#include <kraken/kraken.h>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

using namespace boost;
using namespace std;

namespace kraken
{
  typedef boost::tuples::tuple<int, int> code_item_location_t;

  class CodeBlockItemInterface
  {
    public:
      virtual ~CodeBlockItemInterface() = 0;

      virtual va_t va() = 0;
      virtual string to_string(const string& format = "") = 0;
      virtual bool is_branch() = 0;
      virtual const vector<code_item_location_t>& go_to() = 0;
  };
}

inline kraken::CodeBlockItemInterface::~CodeBlockItemInterface()
{
}

#endif
