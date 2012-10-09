#include "CodeContainer.h"

using namespace std;

// OPTIMIZE: code chunks are stored in vector, which causes extra operations when finding intersections between chunks 
// because we are removing them from vector, combining them in one chunk and re-adding it to vector. This may cause 
// some problems and it may be a good idea to combine them inside an existing code chunk or use list container instead of vector

CodeContainer::CodeContainer(
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

    // Checking if disassembled code chunk intersects with any of the previously disassembled ones
    if( ( iteratorToIntersection ) == _codeCollection.end()
        || ( iteratorToIntersection->includes( disassembledCodeChunk ) ) )
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

  sort( _codeCollection.begin(), _codeCollection.end(), [] (const CodeChunk& firstChunk, const CodeChunk& secondChunk)
    {
      return firstChunk.first_rva() < secondChunk.first_rva();
    } );
};

CodeChunk CodeContainer::disassemble_code_chunk(queue<DISASM>& jumpInstructionQueue)
{
  int instructionLength;
  CodeChunk codeChunk;

  auto isEndOfCodeBlock = false;
  auto disasm = jumpInstructionQueue.front();

  while (!isEndOfCodeBlock)
  {
    instructionLength = Disasm(&disasm);

    if ( ( instructionLength != OUT_OF_BLOCK ) && ( instructionLength != UNKNOWN_OPCODE ) )
    {
      if ( ( disasm.Instruction.BranchType == JmpType ) && ( disasm.Instruction.AddrValue != 0 ) )
      {
        DISASM tempDisasm;

        tempDisasm.EIP = rva_to_offset( (int) disasm.Instruction.AddrValue - 0x400000 );
        tempDisasm.VirtualAddr = disasm.Instruction.AddrValue;

        jumpInstructionQueue.push( tempDisasm );
      }

      codeChunk.add_to_chunk( disasm );
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
int CodeContainer::rva_to_offset(const int& rva)
{
  int rawSize,
    virtualBorneInf,
    rawBorneInf,
    sectionHeader,
    offsetNtHeaders,
    offsetSectionHeaders,
    numberOfSections,
    sizeOfOptionalHeaders,
    virtualAddress;

  // Calculating offset to get through section headers
  offsetNtHeaders = (int) *( (int*) ( &_codeBuff.front() + 0x3c ) );
  sizeOfOptionalHeaders = (int) *( (unsigned short*) ( &_codeBuff.front() + offsetNtHeaders + 0x14 ) );
  offsetSectionHeaders = offsetNtHeaders + sizeOfOptionalHeaders + 0x18;

  numberOfSections = (int) *( (unsigned short*) ( &_codeBuff.front() + offsetNtHeaders + 6) ); // Wtf do we need this for? Can't find any use of this variable.

  virtualBorneInf = 0;
  rawBorneInf = 0;
  virtualAddress = 0;
  sectionHeader = 0;

  while ( virtualAddress <= rva )
  {
    if ( virtualAddress != 0 )
    {
      virtualBorneInf = virtualAddress;
      rawSize = (int) *( (unsigned int*) ( &_codeBuff.front() + offsetSectionHeaders + 0x10 ) ); // Why the fuck we are reassigning this shit every time in the cycle?
      rawBorneInf = (int) *( (unsigned int*) ( &_codeBuff.front() + offsetSectionHeaders + 0x14 ) );
    }

    virtualAddress = (int) *( (unsigned int*) ( &_codeBuff.front()  + offsetSectionHeaders + sectionHeader*0x28 + 0x0C ) );
    sectionHeader ++;
  }

  if ( ( rva - virtualBorneInf ) > rawSize )
  {
    return -1;
  }

  rawBorneInf = rawBorneInf >> 8;

  if ( rawBorneInf & 1 )
  { 
    rawBorneInf--;
  }

  rawBorneInf = rawBorneInf << 8;

  return rva - virtualBorneInf + rawBorneInf + (int) &_codeBuff.front();
};

CodeContainer::code_collection_t::iterator CodeContainer::check_if_intersects(const CodeChunk& codeChunk)
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

void CodeContainer::merge_code_chunks(CodeChunk& destination, const CodeChunk& firstCodeChunk, const CodeChunk& secondCodeChunk)
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