#include "include/kraken/Disassembler.h"

#include <algorithm>
#include <boost/log/trivial.hpp>
#include <map>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace kraken;

bool Disassembler::do_work(const Decoder& decoder)
{
  queue<va_t> jumpInstructionQueue;
  jumpInstructionQueue.push( decoder.entry_point() );

  while( false == jumpInstructionQueue.empty() )
  {
    disassemble_next_jump( decoder, jumpInstructionQueue );
  }

  fill_code_collection_using_instruction_map();

  log_instruction_statics();

  return true;
}

void Disassembler::log_instruction_statics()
{
  std::map<UInt32, string> stringOpcodeMap;
  std::map<UInt32, int> countOpcodesMap;

  go_through_instructions([&stringOpcodeMap, &countOpcodesMap](const AsmCode& asmCode)
  {
    UInt32 opcode = asmCode.Instruction.Opcode;

    if( stringOpcodeMap.end() == stringOpcodeMap.find( opcode ) )
    {
      stringOpcodeMap[opcode] = asmCode.Instruction.Mnemonic;
      countOpcodesMap[opcode] = 1;
    }
    else
    {
      countOpcodesMap[opcode] += 1;
    }
  });

  stringstream listOfOpcodesStream;

  listOfOpcodesStream << "\tOpcode\t\tMnemonic\t\tCount" << endl;
  for( auto it = stringOpcodeMap.begin(), end = stringOpcodeMap.end();
       it != end;
       ++it)
  {
    listOfOpcodesStream << "\t0x" << std::hex << std::setw(2) << std::setfill('0') << it->first
                  << "\t\t" << std::setw(9) << std::setfill(' ') << it->second
                  << "\t\t" << std::dec << countOpcodesMap[it->first] << endl;
  }

  BOOST_LOG_TRIVIAL(info) << ">>> Instruction statistics <<<" << endl
                          << "Count of instructions: " << _instructionMap.size() << endl
                          << "Count of unique opcodes: " << stringOpcodeMap.size() << endl
                          << "List of opcodes: " << endl
                          << listOfOpcodesStream.str();

}

void Disassembler::disassemble_next_jump(const Decoder& decoder, queue<va_t>& jumpInstructionQueue)
{
  AsmCode currentAsmCode;

  va_t instrVirtAddr = jumpInstructionQueue.front();
  jumpInstructionQueue.pop();

  if( is_instruct_decoded( instrVirtAddr ) )
  {
    return;
  }

  while( decoder.decode(instrVirtAddr, &currentAsmCode ) )
  {
    _instructionMap.insert(currentAsmCode.VirtualAddr, new AsmCode( currentAsmCode ) );

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

    instrVirtAddr += currentAsmCode.length;
  }
}

bool Disassembler::is_instruct_decoded( va_t address )
{
  return _instructionMap.find( address ) != _instructionMap.end();
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

void Disassembler::fill_code_collection_using_instruction_map()
{
  _codeCollection.clear();

  if( _instructionMap.empty() )
  {
    return;
  }

  AsmCode* firstAsmCode = _instructionMap.begin()->second;
  va_t lastVirtAddr = firstAsmCode->last_va();

  auto beginOfNewChunk = _instructionMap.begin();
  for( auto it = ++_instructionMap.begin(), end = _instructionMap.end(); it != end; ++it)
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
  _codeCollection.push_back( create_code_chunk( beginOfNewChunk, _instructionMap.end() ) );
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
