#ifndef __H_CODE_CHUNK_MAP__
#define __H_CODE_CHUNK_MAP__

#include "CodeChunk.h"

#include <map>

namespace kraken
{
  class CodeChunkMap
  {
    public:
      // internal classes definition
      struct ChunkLocation
      {
        // fields
        va_t _first;

        va_t _last;

        // ctors
        ChunkLocation();

        ChunkLocation( va_t first, va_t last );

        ChunkLocation( const CodeChunk& codeChunk );

        // functions
        bool intersects_with( const ChunkLocation& another ) const;
      };

      // functions
      void add( const ChunkLocation& location );

      void remove( const ChunkLocation& location );
      
      size_t size();

      bool includes( va_t address );


    private:
      // fields
      std::map<ChunkLocation, int> _map;
  };

  inline bool operator <( const CodeChunkMap::ChunkLocation& one, const CodeChunkMap::ChunkLocation& another )
  {
    if( one.intersects_with( another ) )
    {
      return false;
    }

    return one._first < another._first;
  }
}

#endif
