#include "include/kraken/Disassembler.h"

#include <algorithm>

using namespace std;
using namespace kraken;

// OPTIMIZE: code chunks are stored in vector, which causes extra operations when finding intersections between chunks 
// because we are removing them from vector, combining them in one chunk and re-adding it to vector. This may cause 
// some problems and it may be a good idea to combine them inside an existing code chunk or use list container instead of vector

bool Disassembler::fill(const Decoder& decoder)
{
  queue<va_t> jumpInstructionQueue;
  jumpInstructionQueue.push( decoder.entry_point() );

  while( jumpInstructionQueue.size() != 0 )
  {
    //_ASSERT( _chunkMap.includes( jumpInstructionQueue.front() ) == is_instruct_decoded( jumpInstructionQueue.front() ) );

    if( _chunkMap.includes( jumpInstructionQueue.front() ) )
    {
      jumpInstructionQueue.pop();
      continue;
    }

    auto disassembledChunk = disassemble_next_code_chunk( jumpInstructionQueue, decoder );

    auto iteratorToIntersection = check_if_intersects( disassembledChunk );

    // Checking if disassembled code chunk intersects with any of the previously disassembled ones    
    if( iteratorToIntersection == _codeCollection.end() )
    {
      _codeCollection.push_back( disassembledChunk );

      // Add to chunk map. Don't forget to delete from chunk map when delete from code collection
      _chunkMap.add( disassembledChunk );
    }
    else
    {
      if( iteratorToIntersection->includes( disassembledChunk ) ) 
      {
        continue;
      }
      else if( disassembledChunk.includes( *iteratorToIntersection ) )
      {
        _chunkMap.remove( *iteratorToIntersection );

        _codeCollection.erase( iteratorToIntersection );
        _codeCollection.push_back( disassembledChunk );

        _chunkMap.add( disassembledChunk );
      }
      else
      {
        CodeChunk mergedCodeChunk;

        merge_code_chunks( mergedCodeChunk, *iteratorToIntersection, disassembledChunk );

        _codeCollection[ iteratorToIntersection - _codeCollection.begin() ] = mergedCodeChunk;
      }
    }
  }

  sort( _codeCollection.begin(), _codeCollection.end(), [] (const CodeChunk& firstChunk, const CodeChunk& secondChunk)
  {
    return firstChunk.first_va() < secondChunk.first_va();
  } );

  return true;
}

CodeChunk Disassembler::disassemble_next_code_chunk(queue<va_t>& jumpInstructionQueue, const Decoder& decoder)
{
  CodeChunk codeChunk = decoder.decode_chunk( jumpInstructionQueue.front() );
  jumpInstructionQueue.pop();

  for( auto currentAsmCode = codeChunk.begin(), endAsmCode = codeChunk.end(); currentAsmCode != endAsmCode; ++currentAsmCode )
  {
    switch(currentAsmCode->Instruction.BranchType)
    {
      case JO:
      case JC:
      case JE:
      case JA:
      case JS:
      case JP:
      case JL:
      case JG:
      case JB:
      case JECXZ:
      case JmpType:
      case CallType:
      case JNO:
      case JNC:
      case JNE:
      case JNA:
      case JNS:
      case JNP:
      case JNL:
      case JNG:
      case JNB:
        if( 0 != currentAsmCode->Instruction.AddrValue )
        {
          jumpInstructionQueue.push( currentAsmCode->Instruction.AddrValue );
        }
    }
  }

  return codeChunk;
}

bool Disassembler::is_instruct_decoded( va_t address )
{
  for( auto chunk : _codeCollection )
  {
    if( chunk.is_address_included( address ) )
    {
      return true;
    }
  }
  return false;
}

Disassembler::code_collection_t::iterator Disassembler::check_if_intersects(const CodeChunk& codeChunk)
{
  for( auto it = _codeCollection.begin(), end = _codeCollection.end(); it != end; ++it )
  {
    if( it->intersects_with( codeChunk ) )
    {
      return it;
    }
  }

  return _codeCollection.end();
}

void Disassembler::merge_code_chunks(CodeChunk& resultChunk,
                                     const CodeChunk& firstCodeChunk,
                                     const CodeChunk& secondCodeChunk)
{
  const CodeChunk* endChunk;

  if( firstCodeChunk.first_va() < secondCodeChunk.first_va() )
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
}

void Disassembler::go_through_chunks(std::function<void (const CodeChunk&)> process_chunk) const
{
  for(auto chunkIt = begin(), endIt = end(); chunkIt != endIt; ++chunkIt )
  {
    process_chunk(*chunkIt);
  }
}

void Disassembler::go_through_instructions(std::function<void (const AsmCode&)> process_instr) const
{
  for(auto chunkIt = begin(), endIt = end(); chunkIt != endIt; ++chunkIt )
  {
    for(vector<AsmCode>::const_iterator instrIt = chunkIt->begin(), endInstrIt = chunkIt->end();
        instrIt != endInstrIt;
        ++instrIt)
    {
      process_instr(*instrIt);
    }
  }
}

int Disassembler::get_chunk_count() const
{
  return _codeCollection.size();
}

int Disassembler::get_instruction_count() const
{
  int instructionCount = 0;
  for(auto &codeChunk : _codeCollection) {
    instructionCount += codeChunk.size();
  }

  return instructionCount;
}
