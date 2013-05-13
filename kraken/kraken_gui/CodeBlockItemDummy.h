#ifndef __H_CODE_BLOCK_ITEM_DUMMY__
#define __H_CODE_BLOCK_ITEM_DUMMY__

#include <kraken/CodeBlockItemInterface.h>

#include "CodeBlockItemDummy.h"

using namespace kraken;

class CodeBlockItemDummy : public CodeBlockItemInterface
{
  public:
    ~CodeBlockItemDummy() {}

    CodeBlockItemDummy();

    va_t va();
    string to_string(const string& format = "");
    bool is_branch();
    const vector<codeItemLocation_t>& go_to();

  private:
    vector<codeItemLocation_t> _goToArray;
};


CodeBlockItemDummy::CodeBlockItemDummy()
{
  _goToArray.push_back( codeItemLocation_t( 8, 10 ) );
}

va_t CodeBlockItemDummy::va()
{
  return 40012;
}

string CodeBlockItemDummy::to_string(const string& format)
{
  return "some code item";
}

bool CodeBlockItemDummy::is_branch()
{
  return true;
}

const vector<codeItemLocation_t>& CodeBlockItemDummy::go_to()
{
  return _goToArray;
}

#endif
