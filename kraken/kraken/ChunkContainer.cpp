#include "ChunkContainer.h"

using namespace std;
using namespace kraken;

// OPTIMIZE: code chunks are stored in vector, which causes extra operations when finding intersections between chunks 
// because we are removing them from vector, combining them in one chunk and re-adding it to vector. This may cause 
// some problems and it may be a good idea to combine them inside an existing code chunk or use list container instead of vector

ChunkContainer::ChunkContainer(const vector<unsigned char> &memBuff, size_t startCodeSection, rva_t virtualAddress)
{
  fill( memBuff, startCodeSection, virtualAddress );
}

bool ChunkContainer::fill(
  const vector<unsigned char> &memBuff, 
  size_t startCodeSection, 
  rva_t virtualAddress)
{
  if( memBuff.empty() )
  {
    return false;
  }

  _codeBuff = memBuff;

  AsmCode disasm;
  disasm.Eip = startCodeSection;
  disasm.VirtualAddr = virtualAddress;
  disasm.Archi = 0;

  queue<AsmCode> jumpInstructionQueue; 
  jumpInstructionQueue.push( disasm );

  PeDisassembler disassembler;
  while( jumpInstructionQueue.size() != 0 )
  {
    auto disassembledCodeChunk = disassemble_code_chunk( jumpInstructionQueue, &disassembler );

    auto iteratorToIntersection = check_if_intersects( disassembledCodeChunk );

    // Checking if disassembled code chunk intersects with any of the previously disassembled ones
    if( ( ( iteratorToIntersection ) == _codeCollection.end() )
      || ( ( iteratorToIntersection->includes( disassembledCodeChunk ) ) ) )
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

  return true;
};

CodeChunk ChunkContainer::disassemble_code_chunk( queue<AsmCode>& jumpInstructionQueue, const Disassembler* disassembler )
{
  CodeChunk codeChunk;
  auto disasmResult = jumpInstructionQueue.front();

  for( int instructionLength = disassembler->disassemble( &disasmResult );
    ( instructionLength != OUT_OF_BLOCK ) && ( instructionLength != UNKNOWN_OPCODE );
    instructionLength = disassembler->disassemble( &disasmResult ) )
  {
    if ( ( disasmResult.Instruction.BranchType == JmpType ) && ( disasmResult.Instruction.AddrValue != 0 ) )
    {
      AsmCode tempDisasm;

      tempDisasm.Eip = rva_to_offset( (int) disasmResult.Instruction.AddrValue - 0x400000 );
      tempDisasm.VirtualAddr = disasmResult.Instruction.AddrValue;

      jumpInstructionQueue.push( tempDisasm );
    }

    codeChunk.add_to_chunk( disasmResult );
    disasmResult.Eip = disasmResult.Eip + instructionLength;
    disasmResult.VirtualAddr = disasmResult.VirtualAddr + instructionLength;
  }

  jumpInstructionQueue.pop();

  return codeChunk;
};

// TODO: rename variables and get rid of all that needless dark magic if possible
int ChunkContainer::rva_to_offset(const int& rva)
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

int RVA2OFFSET(int RVA, unsigned char *pBuff)
{
  int RawSize, VirtualBorneInf, RawBorneInf, SectionHeader;
  int OffsetNtHeaders,OffsetSectionHeaders, 
    NumberOfSections, SizeOfOptionalHeaders, VirtualAddress;

  OffsetNtHeaders = (int) *((int*) (pBuff + 0x3c));
  NumberOfSections = (int) *((unsigned short*) (pBuff + OffsetNtHeaders + 6));
  SizeOfOptionalHeaders = (int) *((unsigned short*) (pBuff + OffsetNtHeaders + 0x14));

  OffsetSectionHeaders = OffsetNtHeaders + SizeOfOptionalHeaders + 0x18;

  VirtualBorneInf = 0;
  RawBorneInf = 0;
  VirtualAddress = 0;
  SectionHeader = 0;
  while (VirtualAddress <= RVA) {
    if (VirtualAddress != 0) {
      VirtualBorneInf = VirtualAddress;
      RawSize = (int) *((unsigned int*) (pBuff + OffsetSectionHeaders + 0x10));
      RawBorneInf = (int) *((unsigned int*) (pBuff + OffsetSectionHeaders + 0x14));
    }
    VirtualAddress = (int) *((unsigned int*) (pBuff + OffsetSectionHeaders 
      + SectionHeader*0x28 + 0x0C));
    SectionHeader ++;
  }
  if ((RVA-VirtualBorneInf)>RawSize) return -1;
  RawBorneInf = RawBorneInf >> 8;
  if (RawBorneInf & 1) RawBorneInf--;
  RawBorneInf = RawBorneInf << 8;
  return RVA - VirtualBorneInf + RawBorneInf + (int) pBuff;
}

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

void ChunkContainer::merge_code_chunks(CodeChunk& destination, const CodeChunk& firstCodeChunk, const CodeChunk& secondCodeChunk)
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