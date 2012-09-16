#include "InstructionContainer.h"

using namespace std;

// OPTIMIZE: code chunks are stored in vector, which causes extra operations when finding intersections between chunks 
// because we are removing them from vector, combining them in one chunk and re-adding it to vector. This may cause 
// some problems and it may be a good idea to combine them inside an existing code chunk or use list container instead of vector

InstructionContainer::InstructionContainer(
  const vector<unsigned char> &memBuff, 
  size_t startCodeSection, 
  CodeChunk::rva_t virtualAddress)
{
  _codeBuff = memBuff;

  queue<DISASM> jumpInstructionQueue;

  DISASM disasm;
  disasm.EIP = startCodeSection;
  disasm.VirtualAddr = virtualAddress;
  disasm.Archi = 0;

  jumpInstructionQueue.push( disasm );

  while( jumpInstructionQueue.size() != 0 )
  {
    auto disassembledCodeChunk = disassemble_code_chunk( jumpInstructionQueue );

    auto iteratorToIntersection = check_if_intersects( disassembledCodeChunk );

    if( (iteratorToIntersection) != _codeCollection.end() ||
      ( iteratorToIntersection->includes(disassembledCodeChunk) ) )
    {
      continue;
    }
    if( disassembledCodeChunk.includes( *iteratorToIntersection ) )
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
};

CodeChunk InstructionContainer::disassemble_code_chunk(queue<DISASM>& jumpInstructionQueue)
{
  int instructionLength;
  CodeChunk codeChunk;

  auto isEndOfCodeBlock = false;
  auto disasm = jumpInstructionQueue.front();  

  while (!isEndOfCodeBlock)
  {
    instructionLength = Disasm(&disasm);

    if ((instructionLength != OUT_OF_BLOCK) && (instructionLength != UNKNOWN_OPCODE))
    {
      if ((disasm.Instruction.BranchType == JmpType)
        && (disasm.Instruction.AddrValue != 0))
      {
        DISASM tempDisasm;

        tempDisasm.EIP = rva_to_offset((int) disasm.Instruction.AddrValue - 0x400000);
        tempDisasm.VirtualAddr = disasm.Instruction.AddrValue;

        jumpInstructionQueue.push(tempDisasm);
      }

      codeChunk.add_to_chunk(disasm);
      disasm.EIP = disasm.EIP + instructionLength;
      disasm.VirtualAddr = disasm.VirtualAddr + instructionLength;
    }
    else
    {
      isEndOfCodeBlock = true;
    }
  }

  jumpInstructionQueue.pop();

  return codeChunk;
};

// TODO: rename variables and get rid of all that needless dark magic if possible
int InstructionContainer::rva_to_offset(const int& rva)
{
  int RawSize,
    VirtualBorneInf,
    RawBorneInf,
    SectionHeader,
    OffsetNtHeaders,
    OffsetSectionHeaders,
    NumberOfSections,
    SizeOfOptionalHeaders,
    VirtualAddress;

  // Calculating offset to get through section headers
  OffsetNtHeaders = (int) *((int*) (&(*_codeBuff.begin()) + 0x3c));
  SizeOfOptionalHeaders = (int) *((unsigned short*) (&(*_codeBuff.begin())  + OffsetNtHeaders + 0x14));
  OffsetSectionHeaders = OffsetNtHeaders + SizeOfOptionalHeaders + 0x18;

  NumberOfSections = (int) *((unsigned short*) (&(*_codeBuff.begin())  + OffsetNtHeaders + 6)); // Wtf do we need this for? Can't find any use of this variable.

  VirtualBorneInf = 0;
  RawBorneInf = 0;
  VirtualAddress = 0;
  SectionHeader = 0;

  while (VirtualAddress <= rva)
  {
    if (VirtualAddress != 0)
    {
      VirtualBorneInf = VirtualAddress;
      RawSize = (int) *((unsigned int*) (&(*_codeBuff.begin())  + OffsetSectionHeaders + 0x10)); // Why the fuck we are reassigning this shit every time in the cycle?
      RawBorneInf = (int) *((unsigned int*) (&(*_codeBuff.begin())  + OffsetSectionHeaders + 0x14));
    }

    VirtualAddress = (int) *((unsigned int*) (&(*_codeBuff.begin())  + OffsetSectionHeaders 
      + SectionHeader*0x28 + 0x0C));
    SectionHeader ++;
  }

  if ((rva - VirtualBorneInf) > RawSize) 
  {
    return -1;
  }

  RawBorneInf = RawBorneInf >> 8;

  if (RawBorneInf & 1) 
  { 
    RawBorneInf--;
  }

  RawBorneInf = RawBorneInf << 8;

  return rva - VirtualBorneInf + RawBorneInf + (int) &(*_codeBuff.begin()) ;
};

InstructionContainer::code_collection_t::iterator InstructionContainer::check_if_intersects(const CodeChunk& codeChunk)
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

void InstructionContainer::merge_code_chunks(CodeChunk& destination, const CodeChunk& firstCodeChunk, const CodeChunk& secondCodeChunk)
{
  const CodeChunk* endChunk;

  if( firstCodeChunk.first_rva() < secondCodeChunk.first_rva() )
  {
    destination = firstCodeChunk;
    endChunk = &secondCodeChunk;
  }
  else
  {
    destination = secondCodeChunk;
    endChunk = &firstCodeChunk;
  }

  auto it = destination.begin();
  for( auto end = endChunk->end(); it != end; ++it )
  {
    if( it->VirtualAddr == destination.back().VirtualAddr )
    {
      break;
    }
  }

  if( it != destination.end() )
  {
    destination.add_to_chunk( it, endChunk->end() );
  }
};