#include "include/kraken/CodeChunk.h"

using namespace std;

bool CodeChunk::operator == ( const CodeChunk &otherChunk ) const
{
  return this->_codeChunk == otherChunk._codeChunk;
}
