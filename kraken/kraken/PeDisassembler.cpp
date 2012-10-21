#include "PeDisassembler.h"

#include <boost\filesystem.hpp>

#include <BeaEngine\BeaEngine.h>

using namespace std;
namespace fs = boost::filesystem;

int PeDisassembler::disassemble(AsmCode *disasmResult) const
{
  DISASM disasm;

  disasm.EIP = disasmResult->Eip;
  disasm.VirtualAddr = disasmResult->VirtualAddr;
  
  int length = Disasm( &disasm );

  memcpy( disasmResult->CompleteInstr, disasm.CompleteInstr, sizeof( disasmResult->CompleteInstr ) );
  disasmResult->Eip = disasm.EIP;
  disasmResult->VirtualAddr = disasm.VirtualAddr;
  disasmResult->Instruction.AddrValue = disasm.Instruction.AddrValue;
  disasmResult->Instruction.BranchType = (BranchType)disasm.Instruction.BranchType;
  disasmResult->Archi = disasm.Archi;

  return length;
}

////////////////////////////////////////////////////
// public members
bool PeDisassembler::load( const std::string &path )
{
  if( false == load_file_in_filebuf( path ) )
  {
    _fileBuf = vector<unsigned char>();
    return false;
  }

  if( false == is_it_pe_file() )
  {
    _fileBuf = vector<unsigned char>();
    return false;
  }

  if( false == fill_pe_struct_fields() )
  {
    return false;
  }

  return true;
}

const unsigned char* PeDisassembler::buf()
{
  return ( _fileBuf.empty() ? ( (const unsigned char*) NULL ) : &(_fileBuf[ 0 ]) );
}

//TODO validate that fileBuf contains the last byte of T type (check sizeof(T) ).
template <typename T>
const T* PeDisassembler::buf( size_t offset )
{
  return ( offset >= _fileBuf.size() ? (const T*) NULL : (const T*) &(_fileBuf[ offset ]) );
}

////////////////////////////////////////////////////
// private members
bool PeDisassembler::is_it_pe_file()
{
  const IMAGE_DOS_HEADER *dosHeader = buf<IMAGE_DOS_HEADER>( 0 );

  if( NULL == dosHeader )
  {
    cout << "File too small. Can't read dos header." << endl;
    return false;
  }

  if( IMAGE_DOS_SIGNATURE != dosHeader->e_magic )
  {
    cout << "File isn't PE file. It hasn't got IMAGE_DOS_SIGNATURE." << endl;
    return false;
  }

  const IMAGE_NT_HEADERS32 *imageNtHeader32;

  if( NULL == ( imageNtHeader32 = buf<IMAGE_NT_HEADERS32>( dosHeader->e_lfanew ) ) )
  {
    cout << "File too small. Can't read image NT headers 32." << endl;
    return false;
  }

  if( IMAGE_NT_SIGNATURE != imageNtHeader32->Signature )
  {
    cout << "File isn't PE file. It hasn't got IMAGE_NT_SIGNATURE." << endl;
    return false;
  }

  return true;
}

bool PeDisassembler::fill_pe_struct_fields()
{
  const IMAGE_DOS_HEADER *dosHeader = buf<IMAGE_DOS_HEADER>( 0 );
  size_t ntHeaderOffset = dosHeader->e_lfanew;

  _imageNtHeader32 = buf<IMAGE_NT_HEADERS32>( ntHeaderOffset );
  size_t sectionHeadersOffset = sizeof( IMAGE_NT_HEADERS32 ) + ntHeaderOffset;

  _sectionHeaders.SectionHeaders = buf<IMAGE_SECTION_HEADER>( sectionHeadersOffset );

  if( NULL == _sectionHeaders.SectionHeaders )
  {
    return false;
  }

  _sectionHeaders.NumberOfSections = _imageNtHeader32->FileHeader.NumberOfSections;

  size_t sizeOfImageSectionHeader = sizeof(IMAGE_SECTION_HEADER);

  return true;
}

bool PeDisassembler::load_file_in_filebuf( const std::string &fileName )
{
  fs::path filePath( fileName );

  if( false == fs::exists( filePath ) )
  {
    cout << fileName << " doesn't exist." << endl;
    return false;
  }

  if( false == fs::is_regular_file( filePath ) )
  {
    cout << fileName << " isn't file." << endl;
    return false;
  }

  if( true == fs::is_empty( filePath ) )
  {
    cout << fileName << " is empty." << endl;
  }

  ifstream fileStream( fileName, ios::binary | ios::in );
  boost::uintmax_t fileSize = fs::file_size( filePath );
  typedef std::istream_iterator<unsigned char> istream_iterator;

  _fileBuf = vector<unsigned char>();
  _fileBuf.reserve( fileSize );

  fileStream >> std::noskipws;
  copy( istream_iterator(fileStream),
        istream_iterator(),
			  std::back_inserter(_fileBuf));

  return true;
}

CodeChunk PeDisassembler::disassemble_code_chunk( rva_t instrAddr )
{
  CodeChunk codeChunk;

  AsmCode tempDisasm;

  tempDisasm.Eip = (size_t)( buf() + rva_to_offset( instrAddr ) );
  tempDisasm.VirtualAddr = instrAddr;
  tempDisasm.Archi = 0;

  for( int instructionLength = disassemble( &tempDisasm );
       ( instructionLength != kraken::OUT_OF_BLOCK ) && ( instructionLength != kraken::UNKNOWN_OPCODE );
       instructionLength = disassemble( &tempDisasm ) )
  {
    codeChunk.add_to_chunk( tempDisasm );

    tempDisasm.Eip = tempDisasm.Eip + instructionLength;
    tempDisasm.VirtualAddr = tempDisasm.VirtualAddr + instructionLength;
  }

  return codeChunk;
};

int PeDisassembler::rva_to_offset(rva_t rva)
{
  rva -= _imageNtHeader32->OptionalHeader.ImageBase;

  for( int i = 0; i < _sectionHeaders.NumberOfSections; ++i )
  {
    size_t startVirtAddr = _sectionHeaders.SectionHeaders[ i ].VirtualAddress;
    size_t endVirtAddr = startVirtAddr + _sectionHeaders.SectionHeaders[ i ].SizeOfRawData;

    if( startVirtAddr <= rva && rva < endVirtAddr )
    {
      return rva - startVirtAddr + _sectionHeaders.SectionHeaders[ i ].PointerToRawData;
    }
  }

  return -1;
};

rva_t PeDisassembler::entry_point()
{
  return _imageNtHeader32->OptionalHeader.AddressOfEntryPoint + _imageNtHeader32->OptionalHeader.ImageBase;
}