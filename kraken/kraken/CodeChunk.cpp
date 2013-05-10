#include "include/kraken/CodeChunk.h"

using namespace std;

bool CodeChunk::operator == ( const CodeChunk &otherChunk ) const
{
  return this->_codeChunk == otherChunk._codeChunk;
}

void CodeChunk::go_through_instructions(std::function<void (const AsmCode&)> process_instr) const
{
  for(auto instrIt = begin(), endIt = end(); instrIt != endIt; ++instrIt )
  {
    process_instr(*instrIt);
  }
}
