#include "CodeChunk.h"

using namespace std;

bool CodeChunk::operator==( const CodeChunk &otherChunk )
{
  return 0 == memcmp( this, &otherChunk, sizeof( CodeChunk ) );
}