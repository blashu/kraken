#include "ChunkContainer.h"

#include <algorithm>

using namespace std;
using namespace kraken;

// OPTIMIZE: code chunks are stored in vector, which causes extra operations when finding intersections between chunks 
// because we are removing them from vector, combining them in one chunk and re-adding it to vector. This may cause 
// some problems and it may be a good idea to combine them inside an existing code chunk or use list container instead of vector

ChunkContainer::ChunkContainer(const Disassembler& disassembler)
{
  fill( disassembler );
}

bool ChunkContainer::fill(const Disassembler& disassembler)
{
  queue<rva_t> jumpInstructionQueue;
  jumpInstructionQueue.push( disassembler.entry_point() );

  while( jumpInstructionQueue.size() != 0 )
  {
    auto disassembledCodeChunk = disassemble_next_code_chunk( jumpInstructionQueue, disassembler );

    auto iteratorToIntersection = check_if_intersects( disassembledCodeChunk );

    // Checking if disassembled code chunk intersects with any of the previously disassembled ones    
    if( iteratorToIntersection == _codeCollection.end() )
    {
      _codeCollection.push_back( disassembledCodeChunk );
    }
    else
    {
      if( iteratorToIntersection->includes( disassembledCodeChunk ) ) 
      {
        continue;
      }
      else if( disassembledCodeChunk.includes( *iteratorToIntersection ) )
      {
        _codeCollection.erase( iteratorToIntersection );
        _codeCollection.push_back( disassembledCodeChunk );
      }
      else
      {
        CodeChunk mergedCodeChunk;

        merge_code_chunks( mergedCodeChunk, *iteratorToIntersection, disassembledCodeChunk );

        _codeCollection[ iteratorToIntersection - _codeCollection.begin() ] = mergedCodeChunk;
      }
    }
  }

  sort( _codeCollection.begin(), _codeCollection.end(), [] (const CodeChunk& firstChunk, const CodeChunk& secondChunk)
  {
    return firstChunk.first_rva() < secondChunk.first_rva();
  } );

  return true;
};

CodeChunk ChunkContainer::disassemble_next_code_chunk(queue<rva_t>& jumpInstructionQueue, const Disassembler& disassembler)
{
  CodeChunk codeChunk = disassembler.disassemble_code_chunk( jumpInstructionQueue.front() );

  for( auto currentAsmCode = codeChunk.begin(), endAsmCode = codeChunk.end(); currentAsmCode != endAsmCode; ++currentAsmCode )
  {
    if( JmpType == currentAsmCode->Instruction.BranchType )
    {
      jumpInstructionQueue.push( currentAsmCode->Instruction.AddrValue );
    }
  }

  jumpInstructionQueue.pop();

  return codeChunk;
};

ChunkContainer::code_collection_t::iterator ChunkContainer::check_if_intersects(const CodeChunk& codeChunk)
{
  for( auto it = _codeCollection.begin(), end = _codeCollection.end(); it != end; ++it )
  {
    if( it->intersects_with( codeChunk ) )
    {
      return it;
    }
  }

  return _codeCollection.end();
};

void ChunkContainer::merge_code_chunks(CodeChunk& resultChunk, const CodeChunk& firstCodeChunk, const CodeChunk& secondCodeChunk)
{
  const CodeChunk* endChunk;

  if( firstCodeChunk.first_rva() < secondCodeChunk.first_rva() )
  {
    resultChunk = firstCodeChunk;
    endChunk = &secondCodeChunk;
  }
  else
  {
    resultChunk = secondCodeChunk;
    endChunk = &firstCodeChunk;
  }

  auto iteratorToIntersection = std::find( endChunk->begin(), endChunk->end(), resultChunk.back() );

  resultChunk.add_to_chunk( iteratorToIntersection, endChunk->end() );
};