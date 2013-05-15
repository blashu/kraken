#include "include/kraken/Disassembler.h"

#include <algorithm>

using namespace std;
using namespace kraken;

// OPTIMIZE: code chunks are stored in vector, which causes extra operations when finding intersections between chunks 
// because we are removing them from vector, combining them in one chunk and re-adding it to vector. This may cause 
// some problems and it may be a good idea to combine them inside an existing code chunk or use list container instead of vector

bool Disassembler::do_work(const Decoder& decoder)
{
  queue<va_t> jumpInstructionQueue;
  jumpInstructionQueue.push( decoder.entry_point() );

  while( false == jumpInstructionQueue.empty() )
  {
    disassemble_next_jump( decoder, jumpInstructionQueue );
  }

  fill_code_collection_using_asm_map();

  return true;
}

void Disassembler::disassemble_next_jump(const Decoder& decoder, queue<va_t>& jumpInstructionQueue)
{
  AsmCode currentAsmCode;

  va_t instrVirtAddr = jumpInstructionQueue.front();
  jumpInstructionQueue.pop();

  if( _asmCodeMap.find( instrVirtAddr ) != _asmCodeMap.end() )
  {
    return;
  }

  for( int instructionLength = decoder.decode(instrVirtAddr, &currentAsmCode );
      instructionLength > 0;
      instructionLength = decoder.decode(instrVirtAddr, &currentAsmCode ) )
  {
    _asmCodeMap.insert(currentAsmCode.VirtualAddr, new AsmCode( currentAsmCode ) );

    if( currentAsmCode.Instruction.BranchType == kraken::RetType )
    {
      break;
    }

    if( currentAsmCode.Instruction.BranchType == kraken::JmpType )
    {
      if( 0 != currentAsmCode.Instruction.AddrValue )
      {
        jumpInstructionQueue.push( currentAsmCode.Instruction.AddrValue );
      }
      break;
    }

    // if branch type
    if( 0 != currentAsmCode.Instruction.AddrValue )
    {
      jumpInstructionQueue.push( currentAsmCode.Instruction.AddrValue );
    }

    instrVirtAddr += instructionLength;
  }
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

CodeChunk create_code_chunk( boost::ptr_map<va_t, AsmCode>::const_iterator begin,
                        boost::ptr_map<va_t, AsmCode>::const_iterator end )
{
  CodeChunk newCodeChunk;

  for( auto it = begin; it != end; ++it )
  {
    newCodeChunk.add_to_chunk( *(it->second) );
  }
  return newCodeChunk;
}

void Disassembler::fill_code_collection_using_asm_map()
{
  _codeCollection.clear();

  if( _asmCodeMap.empty() )
  {
    return;
  }

  AsmCode* firstAsmCode = _asmCodeMap.begin()->second;
  va_t lastVirtAddr = firstAsmCode->last_va();

  auto beginOfNewChunk = _asmCodeMap.begin();
  for( auto it = ++_asmCodeMap.begin(), end = _asmCodeMap.end(); it != end; ++it)
  {
    AsmCode* currentAsmCode = it->second;

    if( currentAsmCode->VirtualAddr == ( lastVirtAddr + 1 ) )
    {
      lastVirtAddr += currentAsmCode->length;
    }
    else
    {
      _codeCollection.push_back( create_code_chunk( beginOfNewChunk, it ) );

      beginOfNewChunk = it;
      lastVirtAddr = currentAsmCode->last_va();
    }
  }
  _codeCollection.push_back( create_code_chunk( beginOfNewChunk, _asmCodeMap.end() ) );
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
