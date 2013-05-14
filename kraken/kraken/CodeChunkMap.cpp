#include "include/kraken/CodeChunkMap.h"

using namespace kraken;

/////////////////////////////////////////////////
// internal classes

// ctors
CodeChunkMap::ChunkLocation::ChunkLocation()
{ 
  _first = 0; 
  _last = 0; 
}

CodeChunkMap::ChunkLocation::ChunkLocation( va_t first, va_t last )
{ 
  _first = first; 
  _last = last; 
}

CodeChunkMap::ChunkLocation::ChunkLocation( const CodeChunk& codeChunk )
{
  _first = codeChunk.first_va();
  _last = codeChunk.last_va();
}

// functions
bool CodeChunkMap::ChunkLocation::intersects_with( const ChunkLocation& another ) const
{
  return ( _first <= another._first ) && ( another._first <= _last );
}


/////////////////////////////////////////////////
// functions
void CodeChunkMap::add( const ChunkLocation& location )
{
  _map[ location ] = 0;
}

void CodeChunkMap::remove( const ChunkLocation& location )
{
  for( auto it = _map.find( location ); it != _map.end(); it = _map.find( location ) )
  {
    _map.erase( it );
  }
}

size_t CodeChunkMap::size()
{
  return _map.size();
}

bool CodeChunkMap::includes( va_t address )
{
  auto it = _map.find( ChunkLocation( address, address ) );
  
  return it != _map.end();
}
